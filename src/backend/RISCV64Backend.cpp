#include "backend/RISCV64BackendInternal.h"

#include <algorithm>
#include <utility>

namespace rcc::backend {

BackendError RISCV64Backend::emit_program(const ir::Program& program,
                                          std::ostream& out) {
    bool emitted_anything = false;

    for (const auto& item : program.values.buffer) {
        if (!std::holds_alternative<ir::Value>(item)) {
            continue;
        }
        auto result = emit_global(std::get<ir::Value>(item), out);
        if (!result.ok()) {
            return result;
        }
        emitted_anything = true;
    }

    for (const auto& item : program.funcs.buffer) {
        if (!std::holds_alternative<ir::Function>(item)) {
            continue;
        }
        auto function = std::get<ir::Function>(item);
        if (!function || function->bbs.buffer.empty()) {
            continue;
        }
        if (emitted_anything) {
            out << '\n';
        }
        auto result = emit_function(function, out);
        if (!result.ok()) {
            return result;
        }
        emitted_anything = true;
    }

    if (!out.good()) {
        return {BackendErrorCode::INVALID_OUTPUT, "failed while writing assembly output"};
    }
    return BackendError::success();
}

BackendError RISCV64Backend::emit_global(const ir::Value& global, std::ostream& out) {
    if (!global || global->kind.tag != ir::ValueTag::GLOBAL_ALLOC) {
        return {BackendErrorCode::INVALID_PROGRAM, "program global list contains a non-global value"};
    }

    const auto* ptr = detail::as_pointer(global->ty);
    if (!ptr || !ptr->base) {
        return {BackendErrorCode::INVALID_PROGRAM, "global allocation must have pointer type"};
    }

    const auto& alloc = std::get<ir::GlobalAlloc>(global->kind.data);
    const bool zero_init = is_zero_initializer(alloc.init);
    const std::string symbol = value_symbol(global);

    out << (zero_init ? ".section .bss\n" : ".section .data\n");
    out << ".globl " << symbol << '\n';
    out << ".align " << std::max<std::size_t>(1, type_align(ptr->base)) << '\n';
    out << symbol << ":\n";

    auto result = emit_global_initializer(alloc.init, ptr->base, out);
    if (!result.ok()) {
        return result;
    }
    return BackendError::success();
}

BackendError RISCV64Backend::emit_function(const ir::Function& function, std::ostream& out) {
    if (!function) {
        return {BackendErrorCode::INVALID_PROGRAM, "null function in program"};
    }
    if (function->bbs.buffer.empty()) {
        return BackendError::success();
    }

    FrameLayout frame;
    auto layout_result = build_frame_layout(function, frame);
    if (!layout_result.ok()) {
        return layout_result;
    }

    FunctionContext ctx;
    ctx.function = function.get();
    ctx.frame = std::move(frame);
    auto arg_layout_result = detail::classify_function_arguments(function, ctx.arg_locations);
    if (!arg_layout_result.ok()) {
        return arg_layout_result;
    }

    const std::string symbol = function_symbol(function);
    out << ".section .text\n";
    out << ".globl " << symbol << '\n';
    out << symbol << ":\n";

    auto result = emit_function_prologue(ctx, out);
    if (!result.ok()) {
        return result;
    }

    for (const auto& item : function->bbs.buffer) {
        if (!std::holds_alternative<ir::BasicBlock>(item)) {
            continue;
        }
        result = emit_basic_block(std::get<ir::BasicBlock>(item), ctx, out);
        if (!result.ok()) {
            return result;
        }
    }

    return BackendError::success();
}

BackendError RISCV64Backend::build_frame_layout(const ir::Function& function,
                                                FrameLayout& frame) {
    std::size_t offset = 0;

    for (const auto& bb_item : function->bbs.buffer) {
        if (!std::holds_alternative<ir::BasicBlock>(bb_item)) {
            continue;
        }
        auto bb = std::get<ir::BasicBlock>(bb_item);
        if (!bb) {
            continue;
        }
        for (const auto& inst_item : bb->insts.buffer) {
            if (!std::holds_alternative<ir::Value>(inst_item)) {
                continue;
            }
            auto inst = std::get<ir::Value>(inst_item);
            if (!inst) {
                continue;
            }

            if (inst->kind.tag == ir::ValueTag::CALL) {
                frame.save_ra = true;
                const auto& call = std::get<ir::Call>(inst->kind.data);
                std::vector<ir::Type> arg_types;
                bool has_float_without_prototype = false;
                std::size_t arg_count = 0;
                for (const auto& arg_item : call.args.buffer) {
                    if (!std::holds_alternative<ir::Value>(arg_item)) {
                        continue;
                    }
                    auto arg = std::get<ir::Value>(arg_item);
                    has_float_without_prototype =
                        has_float_without_prototype ||
                        detail::is_float_scalar_type(arg ? arg->ty : nullptr);
                    ++arg_count;
                }

                if (call.callee && call.callee->ty &&
                    call.callee->ty->tag == ir::TypeTag::FUNCTION) {
                    auto fn_type =
                        std::get<std::shared_ptr<ir::TypeKind::function>>(call.callee->ty->data);
                    if (fn_type && !fn_type->params.buffer.empty()) {
                        for (const auto& item : fn_type->params.buffer) {
                            if (std::holds_alternative<ir::Type>(item)) {
                                arg_types.push_back(std::get<ir::Type>(item));
                            }
                        }
                        if (arg_types.size() != arg_count) {
                            return {BackendErrorCode::UNSUPPORTED_IR,
                                    "variadic or mismatched function calls are unsupported"};
                        }
                    } else if (has_float_without_prototype && arg_count != 0) {
                        return {BackendErrorCode::UNSUPPORTED_IR,
                                "floating-point calls without a visible prototype are unsupported"};
                    }
                }

                if (arg_types.empty()) {
                    for (const auto& arg_item : call.args.buffer) {
                        if (!std::holds_alternative<ir::Value>(arg_item)) {
                            continue;
                        }
                        auto arg = std::get<ir::Value>(arg_item);
                        arg_types.push_back(arg ? arg->ty : nullptr);
                    }
                }

                detail::CallArgLayout arg_layout;
                auto classify_result = detail::classify_call_arguments(arg_types, arg_layout);
                if (!classify_result.ok()) {
                    return classify_result;
                }
                frame.outgoing_arg_size =
                    std::max(frame.outgoing_arg_size, arg_layout.stack_arg_size);
            }

            if (!detail::has_result(inst)) {
                continue;
            }

            std::size_t size = 8;
            std::size_t align = 8;
            if (inst->kind.tag == ir::ValueTag::ALLOC) {
                const auto* ptr = detail::as_pointer(inst->ty);
                if (!ptr || !ptr->base) {
                    return {BackendErrorCode::INVALID_PROGRAM, "alloc result must be a pointer"};
                }
                size = type_size(ptr->base);
                align = type_align(ptr->base);
            } else if (inst->ty) {
                size = type_size(inst->ty);
                align = type_align(inst->ty);
                if (size == 0 && inst->ty->tag != ir::TypeTag::UNIT) {
                    return {BackendErrorCode::UNSUPPORTED_TYPE,
                            "instruction result type has unsupported size"};
                }
                if (align == 0) {
                    align = 1;
                }
            }

            offset = detail::align_to(offset, align);
            offset += size;
            frame.slots.emplace(inst.get(), StackSlot{
                                             .offset = offset - size,
                                             .size = size,
                                             .align = align,
                                         });
        }
    }

    offset = detail::align_to(frame.outgoing_arg_size, 8);
    for (auto& [_, slot] : frame.slots) {
        slot.offset += offset;
    }

    std::size_t frame_end = offset;
    for (const auto& [_, slot] : frame.slots) {
        frame_end = std::max(frame_end, slot.offset + slot.size);
    }

    if (frame.save_ra) {
        frame_end = detail::align_to(frame_end, 8);
        frame.ra_offset = frame_end;
        frame_end += 8;
    }

    frame.stack_size = detail::align_to(frame_end, 16);
    return BackendError::success();
}

BackendError RISCV64Backend::emit_function_prologue(const FunctionContext& ctx,
                                                    std::ostream& out) {
    if (ctx.frame.stack_size != 0) {
        out << "  addi sp, sp, -" << ctx.frame.stack_size << '\n';
    }
    if (ctx.frame.save_ra) {
        out << "  sd ra, " << ctx.frame.ra_offset << "(sp)\n";
    }
    return BackendError::success();
}

BackendError RISCV64Backend::emit_function_epilogue(const FunctionContext& ctx,
                                                    std::ostream& out) {
    if (ctx.frame.save_ra) {
        out << "  ld ra, " << ctx.frame.ra_offset << "(sp)\n";
    }
    if (ctx.frame.stack_size != 0) {
        out << "  addi sp, sp, " << ctx.frame.stack_size << '\n';
    }
    out << "  ret\n";
    return BackendError::success();
}

BackendError RISCV64Backend::emit_basic_block(const ir::BasicBlock& bb,
                                              FunctionContext& ctx,
                                              std::ostream& out) {
    if (!bb) {
        return {BackendErrorCode::INVALID_PROGRAM, "null basic block in function"};
    }

    out << block_label(bb, ctx) << ":\n";
    for (const auto& item : bb->insts.buffer) {
        if (!std::holds_alternative<ir::Value>(item)) {
            continue;
        }
        auto result = emit_instruction(std::get<ir::Value>(item), ctx, out);
        if (!result.ok()) {
            return result;
        }
    }
    return BackendError::success();
}

std::string RISCV64Backend::function_symbol(const ir::Function& function) const {
    return detail::strip_global_prefix(function ? function->name : std::nullopt, "anon_fn");
}

std::string RISCV64Backend::value_symbol(const ir::Value& value) const {
    return detail::strip_global_prefix(value ? value->name : std::nullopt, "anon_global");
}

std::string RISCV64Backend::block_label(const ir::BasicBlock& bb, FunctionContext& ctx) {
    if (!bb) {
        return ".Lbb_null";
    }
    auto it = ctx.block_labels.find(bb.get());
    if (it != ctx.block_labels.end()) {
        return it->second;
    }

    std::string label;
    if (bb->name && !bb->name->empty()) {
        label = (ctx.function->name->empty() ? "" : *ctx.function->name) + "_" + detail::strip_global_prefix(bb->name, "");
    } else {
        label = ".Lbb" + std::to_string(ctx.next_block_id++);
    }
    ctx.block_labels.emplace(bb.get(), label);
    return label;
}

BackendError emit_riscv64(const ir::Program& program, std::ostream& out) {
    RISCV64Backend backend;
    return backend.emit_program(program, out);
}

} // namespace rcc::backend
