#include "backend/RISCV64Backend.h"

#include <algorithm>
#include <variant>

namespace rcc::backend {

namespace {

std::size_t align_to(std::size_t value, std::size_t align) {
    if (align == 0) {
        return value;
    }
    const std::size_t remainder = value % align;
    return remainder == 0 ? value : value + (align - remainder);
}

std::string strip_global_prefix(const std::optional<std::string>& name,
                                const std::string& fallback) {
    if (!name || name->empty()) {
        return fallback;
    }
    if ((*name)[0] == '@' || (*name)[0] == '%') {
        return name->substr(1);
    }
    return *name;
}

const ir::TypeKind::pointer* as_pointer(ir::Type type) {
    if (!type || type->tag != ir::TypeTag::POINTER) {
        return nullptr;
    }
    auto ptr = std::get<std::shared_ptr<ir::TypeKind::pointer>>(type->data);
    return ptr.get();
}

const ir::TypeKind::array* as_array(ir::Type type) {
    if (!type || type->tag != ir::TypeTag::ARRAY) {
        return nullptr;
    }
    auto arr = std::get<std::shared_ptr<ir::TypeKind::array>>(type->data);
    return arr.get();
}

bool has_result(const ir::Value& value) {
    if (!value) {
        return false;
    }
    switch (value->kind.tag) {
        case ir::ValueTag::ALLOC:
        case ir::ValueTag::LOAD:
        case ir::ValueTag::GET_PTR:
        case ir::ValueTag::GET_ELEM_PTR:
        case ir::ValueTag::BINARY:
        case ir::ValueTag::CAST:
        case ir::ValueTag::CALL:
            return true;
        default:
            return false;
    }
}

} // namespace

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
        if (emitted_anything) {
            out << '\n';
        }
        auto result = emit_function(std::get<ir::Function>(item), out);
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

    const auto* ptr = as_pointer(global->ty);
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

    FrameLayout frame;
    auto layout_result = build_frame_layout(function, frame);
    if (!layout_result.ok()) {
        return layout_result;
    }

    FunctionContext ctx;
    ctx.function = function.get();
    ctx.frame = std::move(frame);

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
                std::size_t arg_count = 0;
                for (const auto& arg_item : call.args.buffer) {
                    if (std::holds_alternative<ir::Value>(arg_item)) {
                        ++arg_count;
                    }
                }
                if (arg_count > 8) {
                    frame.outgoing_arg_size =
                        std::max(frame.outgoing_arg_size, (arg_count - 8) * 8);
                }
            }

            if (!has_result(inst)) {
                continue;
            }

            std::size_t size = 8;
            std::size_t align = 8;
            if (inst->kind.tag == ir::ValueTag::ALLOC) {
                const auto* ptr = as_pointer(inst->ty);
                if (!ptr || !ptr->base) {
                    return {BackendErrorCode::INVALID_PROGRAM, "alloc result must be a pointer"};
                }
                size = type_size(ptr->base);
                align = type_align(ptr->base);
            } else if (inst->ty) {
                size = std::max<std::size_t>(8, type_size(inst->ty));
                align = std::max<std::size_t>(8, type_align(inst->ty));
            }

            offset = align_to(offset, align);
            offset += size;
            frame.slots.emplace(inst.get(), StackSlot{
                                             .offset = offset - size,
                                             .size = size,
                                             .align = align,
                                         });
        }
    }

    offset = align_to(frame.outgoing_arg_size, 8);
    for (auto& [_, slot] : frame.slots) {
        slot.offset += offset;
    }

    std::size_t frame_end = offset;
    for (const auto& [_, slot] : frame.slots) {
        frame_end = std::max(frame_end, slot.offset + slot.size);
    }

    if (frame.save_ra) {
        frame_end = align_to(frame_end, 8);
        frame.ra_offset = frame_end;
        frame_end += 8;
    }

    frame.stack_size = align_to(frame_end, 16);
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

BackendError RISCV64Backend::emit_instruction(const ir::Value& inst,
                                              FunctionContext& ctx,
                                              std::ostream& out) {
    if (!inst) {
        return {BackendErrorCode::INVALID_PROGRAM, "null instruction in basic block"};
    }

    switch (inst->kind.tag) {
        case ir::ValueTag::RETURN: {
            const auto& ret = std::get<ir::FuncReturn>(inst->kind.data);
            if (ret.value) {
                auto load_result = emit_value_to_reg(ret.value, "a0", ctx, out);
                if (!load_result.ok()) {
                    return load_result;
                }
            }
            return emit_function_epilogue(ctx, out);
        }
        case ir::ValueTag::JUMP: {
            const auto& jump = std::get<ir::Jump>(inst->kind.data);
            out << "  j " << block_label(jump.target, ctx) << '\n';
            return BackendError::success();
        }
        case ir::ValueTag::ALLOC:
            out << "  # alloc lowered to stack slot during frame layout\n";
            return BackendError::success();
        case ir::ValueTag::BRANCH:
        {
            const auto& br = std::get<ir::Branch>(inst->kind.data);
            auto cond_result = emit_value_to_reg(br.cond, "t0", ctx, out);
            if (!cond_result.ok()) {
                return cond_result;
            }
            out << "  bnez t0, " << block_label(br.true_bb, ctx) << '\n';
            out << "  j " << block_label(br.false_bb, ctx) << '\n';
            return BackendError::success();
        }
        case ir::ValueTag::STORE:
        {
            const auto& store = std::get<ir::Store>(inst->kind.data);
            auto value_result = emit_value_to_reg(store.value, "t0", ctx, out);
            if (!value_result.ok()) {
                return value_result;
            }
            auto addr_result = emit_address_to_reg(store.dest, "t1", ctx, out);
            if (!addr_result.ok()) {
                return addr_result;
            }
            ir::Type store_type = store.value ? store.value->ty : nullptr;
            return emit_store_to_address("t0", "t1", store_type, out);
        }
        case ir::ValueTag::LOAD:
        {
            const auto& load = std::get<ir::Load>(inst->kind.data);
            auto addr_result = emit_address_to_reg(load.src, "t0", ctx, out);
            if (!addr_result.ok()) {
                return addr_result;
            }
            auto load_result = emit_load_from_address("t0", inst->ty, "t1", out);
            if (!load_result.ok()) {
                return load_result;
            }
            auto slot_it = ctx.frame.slots.find(inst.get());
            if (slot_it == ctx.frame.slots.end()) {
                return {BackendErrorCode::INTERNAL_ERROR, "missing stack slot for load result"};
            }
            return emit_store_reg_to_slot("t1", slot_it->second, inst->ty, out);
        }
        case ir::ValueTag::BINARY:
        {
            const auto& binary = std::get<ir::Binary>(inst->kind.data);
            auto lhs_result = emit_value_to_reg(binary.lhs, "t0", ctx, out);
            if (!lhs_result.ok()) {
                return lhs_result;
            }
            auto rhs_result = emit_value_to_reg(binary.rhs, "t1", ctx, out);
            if (!rhs_result.ok()) {
                return rhs_result;
            }

            const bool is_i32 = inst->ty && inst->ty->tag == ir::TypeTag::INT32;
            switch (binary.op) {
                case ir::BinaryOp::ADD:
                    out << "  " << (is_i32 ? "addw" : "add") << " t2, t0, t1\n";
                    break;
                case ir::BinaryOp::SUB:
                    out << "  " << (is_i32 ? "subw" : "sub") << " t2, t0, t1\n";
                    break;
                case ir::BinaryOp::MUL:
                    out << "  " << (is_i32 ? "mulw" : "mul") << " t2, t0, t1\n";
                    break;
                case ir::BinaryOp::DIV:
                    out << "  " << (is_i32 ? "divw" : "div") << " t2, t0, t1\n";
                    break;
                case ir::BinaryOp::MOD:
                    out << "  " << (is_i32 ? "remw" : "rem") << " t2, t0, t1\n";
                    break;
                case ir::BinaryOp::AND:
                    out << "  and t2, t0, t1\n";
                    if (is_i32) out << "  addiw t2, t2, 0\n";
                    break;
                case ir::BinaryOp::OR:
                    out << "  or t2, t0, t1\n";
                    if (is_i32) out << "  addiw t2, t2, 0\n";
                    break;
                case ir::BinaryOp::XOR:
                    out << "  xor t2, t0, t1\n";
                    if (is_i32) out << "  addiw t2, t2, 0\n";
                    break;
                case ir::BinaryOp::SHL:
                    out << "  " << (is_i32 ? "sllw" : "sll") << " t2, t0, t1\n";
                    break;
                case ir::BinaryOp::SHR:
                    out << "  " << (is_i32 ? "srlw" : "srl") << " t2, t0, t1\n";
                    break;
                case ir::BinaryOp::SAR:
                    out << "  " << (is_i32 ? "sraw" : "sra") << " t2, t0, t1\n";
                    break;
                case ir::BinaryOp::EQ:
                    out << "  xor t2, t0, t1\n";
                    out << "  seqz t2, t2\n";
                    break;
                case ir::BinaryOp::NOT_EQ:
                    out << "  xor t2, t0, t1\n";
                    out << "  snez t2, t2\n";
                    break;
                case ir::BinaryOp::LT:
                    out << "  slt t2, t0, t1\n";
                    break;
                case ir::BinaryOp::GT:
                    out << "  slt t2, t1, t0\n";
                    break;
                case ir::BinaryOp::LE:
                    out << "  slt t2, t1, t0\n";
                    out << "  xori t2, t2, 1\n";
                    break;
                case ir::BinaryOp::GE:
                    out << "  slt t2, t0, t1\n";
                    out << "  xori t2, t2, 1\n";
                    break;
            }

            auto slot_it = ctx.frame.slots.find(inst.get());
            if (slot_it == ctx.frame.slots.end()) {
                return {BackendErrorCode::INTERNAL_ERROR, "missing stack slot for binary result"};
            }
            return emit_store_reg_to_slot("t2", slot_it->second, inst->ty, out);
        }
        case ir::ValueTag::CALL:
        {
            const auto& call = std::get<ir::Call>(inst->kind.data);
            std::size_t arg_index = 0;
            for (const auto& arg_item : call.args.buffer) {
                if (!std::holds_alternative<ir::Value>(arg_item)) {
                    continue;
                }
                auto arg = std::get<ir::Value>(arg_item);
                auto arg_result = emit_value_to_reg(arg, "t0", ctx, out);
                if (!arg_result.ok()) {
                    return arg_result;
                }
                if (arg_index < 8) {
                    out << "  mv a" << arg_index << ", t0\n";
                } else {
                    out << "  sd t0, " << ((arg_index - 8) * 8) << "(sp)\n";
                }
                ++arg_index;
            }

            out << "  call " << function_symbol(call.callee) << '\n';
            auto slot_it = ctx.frame.slots.find(inst.get());
            if (slot_it != ctx.frame.slots.end() && inst->ty && inst->ty->tag != ir::TypeTag::UNIT) {
                return emit_store_reg_to_slot("a0", slot_it->second, inst->ty, out);
            }
            return BackendError::success();
        }
        case ir::ValueTag::GET_PTR:
        case ir::ValueTag::GET_ELEM_PTR:
        case ir::ValueTag::CAST:
            return {BackendErrorCode::UNSUPPORTED_IR,
                    "pointer arithmetic/cast lowering is not implemented yet"};
        default:
            return {BackendErrorCode::UNSUPPORTED_IR,
                    "encountered unsupported value kind while emitting RV64 assembly"};
    }
}

BackendError RISCV64Backend::emit_global_initializer(const ir::Value& init,
                                                     ir::Type type,
                                                     std::ostream& out) {
    if (!type) {
        return {BackendErrorCode::INVALID_PROGRAM, "global initializer type is missing"};
    }
    if (!init || init->kind.tag == ir::ValueTag::ZERO_INIT) {
        out << "  .zero " << type_size(type) << '\n';
        return BackendError::success();
    }

    if (type->tag == ir::TypeTag::ARRAY) {
        if (init->kind.tag == ir::ValueTag::ZERO_INIT) {
            out << "  .zero " << type_size(type) << '\n';
            return BackendError::success();
        }
        if (init->kind.tag != ir::ValueTag::AGGREGATE) {
            return {BackendErrorCode::UNSUPPORTED_IR,
                    "array global initializer must be aggregate or zeroinit"};
        }

        const auto* arr = as_array(type);
        const auto& agg = std::get<ir::Aggregate>(init->kind.data);
        std::size_t emitted = 0;
        for (const auto& elem : agg.elems.buffer) {
            if (!std::holds_alternative<ir::Value>(elem)) {
                continue;
            }
            auto result = emit_global_initializer(std::get<ir::Value>(elem), arr->base, out);
            if (!result.ok()) {
                return result;
            }
            ++emitted;
        }
        for (; emitted < arr->len; ++emitted) {
            out << "  .zero " << type_size(arr->base) << '\n';
        }
        return BackendError::success();
    }

    auto integer = extract_integer(init);
    if (!integer.has_value()) {
        return {BackendErrorCode::UNSUPPORTED_IR,
                "global initializer is not a supported integer constant"};
    }

    switch (type->tag) {
        case ir::TypeTag::INT8:
            out << "  .byte " << *integer << '\n';
            return BackendError::success();
        case ir::TypeTag::INT16:
            out << "  .half " << *integer << '\n';
            return BackendError::success();
        case ir::TypeTag::INT32:
            out << "  .word " << *integer << '\n';
            return BackendError::success();
        case ir::TypeTag::INT64:
        case ir::TypeTag::POINTER:
            out << "  .dword " << *integer << '\n';
            return BackendError::success();
        default:
            return {BackendErrorCode::UNSUPPORTED_TYPE,
                    "global initializer type is not supported by the RV64 scaffold backend"};
    }
}

std::string RISCV64Backend::function_symbol(const ir::Function& function) const {
    return strip_global_prefix(function ? function->name : std::nullopt, "anon_fn");
}

std::string RISCV64Backend::value_symbol(const ir::Value& value) const {
    return strip_global_prefix(value ? value->name : std::nullopt, "anon_global");
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
        label = strip_global_prefix(bb->name, "");
    } else {
        label = ".Lbb" + std::to_string(ctx.next_block_id++);
    }
    ctx.block_labels.emplace(bb.get(), label);
    return label;
}

BackendError RISCV64Backend::emit_value_to_reg(const ir::Value& value,
                                               const std::string& reg,
                                               FunctionContext& ctx,
                                               std::ostream& out) {
    if (!value) {
        return {BackendErrorCode::INVALID_PROGRAM, "attempted to materialize null value"};
    }

    switch (value->kind.tag) {
        case ir::ValueTag::NUMBER: {
            auto integer = extract_integer(value);
            if (!integer.has_value()) {
                return {BackendErrorCode::UNSUPPORTED_IR, "floating-point constants are not supported"};
            }
            out << "  li " << reg << ", " << *integer << '\n';
            return BackendError::success();
        }
        case ir::ValueTag::ALLOC:
        case ir::ValueTag::GLOBAL_ALLOC:
        case ir::ValueTag::GET_PTR:
        case ir::ValueTag::GET_ELEM_PTR:
            return emit_address_to_reg(value, reg, ctx, out);
        case ir::ValueTag::FUNC_ARG_REF: {
            const auto& ref = std::get<ir::FuncArgRef>(value->kind.data);
            if (ref.index < 8) {
                out << "  mv " << reg << ", a" << ref.index << '\n';
            } else {
                out << "  ld " << reg << ", "
                    << (ctx.frame.stack_size + ((ref.index - 8) * 8)) << "(sp)\n";
            }
            return BackendError::success();
        }
        case ir::ValueTag::LOAD:
        case ir::ValueTag::BINARY:
        case ir::ValueTag::CALL: {
            auto slot_it = ctx.frame.slots.find(value.get());
            if (slot_it == ctx.frame.slots.end()) {
                return {BackendErrorCode::INTERNAL_ERROR, "missing stack slot for computed value"};
            }
            return emit_load_slot_to_reg(slot_it->second, value->ty, reg, out);
        }
        default:
            return {BackendErrorCode::UNSUPPORTED_IR,
                    "value kind cannot be materialized as a scalar register yet"};
    }
}

BackendError RISCV64Backend::emit_address_to_reg(const ir::Value& value,
                                                 const std::string& reg,
                                                 FunctionContext& ctx,
                                                 std::ostream& out) {
    if (!value) {
        return {BackendErrorCode::INVALID_PROGRAM, "attempted to materialize null address"};
    }

    switch (value->kind.tag) {
        case ir::ValueTag::ALLOC: {
            auto slot_it = ctx.frame.slots.find(value.get());
            if (slot_it == ctx.frame.slots.end()) {
                return {BackendErrorCode::INTERNAL_ERROR, "missing stack slot for alloc"};
            }
            out << "  addi " << reg << ", sp, " << slot_it->second.offset << '\n';
            return BackendError::success();
        }
        case ir::ValueTag::GLOBAL_ALLOC:
            out << "  la " << reg << ", " << value_symbol(value) << '\n';
            return BackendError::success();
        case ir::ValueTag::GET_PTR:
        case ir::ValueTag::GET_ELEM_PTR:
            return {BackendErrorCode::UNSUPPORTED_IR,
                    "pointer arithmetic lowering is not implemented yet"};
        default:
            return {BackendErrorCode::UNSUPPORTED_IR,
                    "value kind cannot be materialized as an address"};
    }
}

BackendError RISCV64Backend::emit_load_from_address(const std::string& addr_reg,
                                                    ir::Type type,
                                                    const std::string& dst_reg,
                                                    std::ostream& out) {
    if (!type) {
        return {BackendErrorCode::INVALID_PROGRAM, "load result type is missing"};
    }
    switch (type->tag) {
        case ir::TypeTag::INT8:
            out << "  lb " << dst_reg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        case ir::TypeTag::INT16:
            out << "  lh " << dst_reg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        case ir::TypeTag::INT32:
            out << "  lw " << dst_reg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        case ir::TypeTag::INT64:
        case ir::TypeTag::POINTER:
            out << "  ld " << dst_reg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        default:
            return {BackendErrorCode::UNSUPPORTED_TYPE, "load type is not supported by RV64 backend"};
    }
}

BackendError RISCV64Backend::emit_store_to_address(const std::string& src_reg,
                                                   const std::string& addr_reg,
                                                   ir::Type type,
                                                   std::ostream& out) {
    if (!type) {
        return {BackendErrorCode::INVALID_PROGRAM, "store value type is missing"};
    }
    switch (type->tag) {
        case ir::TypeTag::INT8:
            out << "  sb " << src_reg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        case ir::TypeTag::INT16:
            out << "  sh " << src_reg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        case ir::TypeTag::INT32:
            out << "  sw " << src_reg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        case ir::TypeTag::INT64:
        case ir::TypeTag::POINTER:
            out << "  sd " << src_reg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        default:
            return {BackendErrorCode::UNSUPPORTED_TYPE, "store type is not supported by RV64 backend"};
    }
}

BackendError RISCV64Backend::emit_store_reg_to_slot(const std::string& src_reg,
                                                    const StackSlot& slot,
                                                    ir::Type type,
                                                    std::ostream& out) {
    if (!type) {
        return {BackendErrorCode::INVALID_PROGRAM, "stack store type is missing"};
    }
    switch (type->tag) {
        case ir::TypeTag::INT8:
            out << "  sb " << src_reg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        case ir::TypeTag::INT16:
            out << "  sh " << src_reg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        case ir::TypeTag::INT32:
            out << "  sw " << src_reg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        case ir::TypeTag::INT64:
        case ir::TypeTag::POINTER:
            out << "  sd " << src_reg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        case ir::TypeTag::UNIT:
            return BackendError::success();
        default:
            return {BackendErrorCode::UNSUPPORTED_TYPE, "stack slot type is not supported"};
    }
}

BackendError RISCV64Backend::emit_load_slot_to_reg(const StackSlot& slot,
                                                   ir::Type type,
                                                   const std::string& dst_reg,
                                                   std::ostream& out) {
    if (!type) {
        return {BackendErrorCode::INVALID_PROGRAM, "stack load type is missing"};
    }
    switch (type->tag) {
        case ir::TypeTag::INT8:
            out << "  lb " << dst_reg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        case ir::TypeTag::INT16:
            out << "  lh " << dst_reg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        case ir::TypeTag::INT32:
            out << "  lw " << dst_reg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        case ir::TypeTag::INT64:
        case ir::TypeTag::POINTER:
            out << "  ld " << dst_reg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        default:
            return {BackendErrorCode::UNSUPPORTED_TYPE, "stack load type is not supported"};
    }
}

std::optional<std::int64_t> RISCV64Backend::extract_integer(const ir::Value& value) const {
    if (!value || value->kind.tag != ir::ValueTag::NUMBER) {
        return std::nullopt;
    }

    const auto& number = std::get<ir::Number>(value->kind.data);
    if (std::holds_alternative<ir::Integer8>(number.num)) {
        return std::get<ir::Integer8>(number.num).value;
    }
    if (std::holds_alternative<ir::Integer16>(number.num)) {
        return std::get<ir::Integer16>(number.num).value;
    }
    if (std::holds_alternative<ir::Integer32>(number.num)) {
        return std::get<ir::Integer32>(number.num).value;
    }
    if (std::holds_alternative<ir::Integer64>(number.num)) {
        return std::get<ir::Integer64>(number.num).value;
    }
    return std::nullopt;
}

std::size_t RISCV64Backend::type_size(ir::Type type) const {
    if (!type) {
        return 0;
    }
    switch (type->tag) {
        case ir::TypeTag::INT8: return 1;
        case ir::TypeTag::INT16: return 2;
        case ir::TypeTag::INT32: return 4;
        case ir::TypeTag::INT64: return 8;
        case ir::TypeTag::POINTER: return 8;
        case ir::TypeTag::ARRAY: {
            const auto* arr = as_array(type);
            return arr ? type_size(arr->base) * arr->len : 0;
        }
        case ir::TypeTag::UNIT: return 0;
        case ir::TypeTag::FLOAT:
        case ir::TypeTag::DOUBLE:
        case ir::TypeTag::FUNCTION:
            return 8;
    }
    return 0;
}

std::size_t RISCV64Backend::type_align(ir::Type type) const {
    if (!type) {
        return 1;
    }
    switch (type->tag) {
        case ir::TypeTag::INT8: return 1;
        case ir::TypeTag::INT16: return 2;
        case ir::TypeTag::INT32: return 4;
        case ir::TypeTag::INT64: return 8;
        case ir::TypeTag::POINTER: return 8;
        case ir::TypeTag::ARRAY: {
            const auto* arr = as_array(type);
            return arr ? type_align(arr->base) : 1;
        }
        case ir::TypeTag::UNIT: return 1;
        case ir::TypeTag::FLOAT: return 4;
        case ir::TypeTag::DOUBLE: return 8;
        case ir::TypeTag::FUNCTION: return 8;
    }
    return 1;
}

bool RISCV64Backend::is_zero_initializer(const ir::Value& value) const {
    return !value || value->kind.tag == ir::ValueTag::ZERO_INIT;
}

BackendError emit_riscv64(const ir::Program& program, std::ostream& out) {
    RISCV64Backend backend;
    return backend.emit_program(program, out);
}

} // namespace rcc::backend
