#include "backend/RISCV64BackendInternal.h"

namespace rcc::backend {

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
                if (detail::is_float_scalar_type(ret.value->ty)) {
                    auto load_result = emit_value_to_freg(ret.value, "fa0", ctx, out);
                    if (!load_result.ok()) {
                        return load_result;
                    }
                } else {
                    auto load_result = emit_value_to_reg(ret.value, "a0", ctx, out);
                    if (!load_result.ok()) {
                        return load_result;
                    }
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
        case ir::ValueTag::BRANCH: {
            const auto& br = std::get<ir::Branch>(inst->kind.data);
            auto cond_result = emit_value_to_reg(br.cond, "t0", ctx, out);
            if (!cond_result.ok()) {
                return cond_result;
            }
            out << "  bnez t0, " << block_label(br.true_bb, ctx) << '\n';
            out << "  j " << block_label(br.false_bb, ctx) << '\n';
            return BackendError::success();
        }
        case ir::ValueTag::STORE: {
            const auto& store = std::get<ir::Store>(inst->kind.data);
            auto addr_result = emit_address_to_reg(store.dest, "t1", ctx, out);
            if (!addr_result.ok()) {
                return addr_result;
            }
            ir::Type store_type = store.value ? store.value->ty : nullptr;
            if (detail::is_float_scalar_type(store_type)) {
                auto value_result = emit_value_to_freg(store.value, "ft0", ctx, out);
                if (!value_result.ok()) {
                    return value_result;
                }
                return emit_store_freg_to_address("ft0", "t1", store_type, out);
            }
            auto value_result = emit_value_to_reg(store.value, "t0", ctx, out);
            if (!value_result.ok()) {
                return value_result;
            }
            return emit_store_to_address("t0", "t1", store_type, out);
        }
        case ir::ValueTag::LOAD: {
            const auto& load = std::get<ir::Load>(inst->kind.data);
            auto addr_result = emit_address_to_reg(load.src, "t0", ctx, out);
            if (!addr_result.ok()) {
                return addr_result;
            }
            auto slot_it = ctx.frame.slots.find(inst.get());
            if (slot_it == ctx.frame.slots.end()) {
                return {BackendErrorCode::INTERNAL_ERROR, "missing stack slot for load result"};
            }
            if (detail::is_float_scalar_type(inst->ty)) {
                auto load_result = emit_load_from_address_to_freg("t0", inst->ty, "ft0", out);
                if (!load_result.ok()) {
                    return load_result;
                }
                return emit_store_freg_to_slot("ft0", slot_it->second, inst->ty, out);
            }
            auto load_result = emit_load_from_address("t0", inst->ty, "t1", out);
            if (!load_result.ok()) {
                return load_result;
            }
            return emit_store_reg_to_slot("t1", slot_it->second, inst->ty, out);
        }
        case ir::ValueTag::BINARY: {
            const auto& binary = std::get<ir::Binary>(inst->kind.data);
            if (detail::is_float_binary_instruction(inst)) {
                if (!detail::is_float_scalar_type(binary.lhs ? binary.lhs->ty : nullptr) ||
                    !detail::is_float_scalar_type(binary.rhs ? binary.rhs->ty : nullptr) ||
                    binary.lhs->ty->tag != binary.rhs->ty->tag) {
                    return {BackendErrorCode::INVALID_PROGRAM,
                            "floating-point binary operands must have matching FLOAT/DOUBLE types"};
                }

                auto slot_it = ctx.frame.slots.find(inst.get());
                if (slot_it == ctx.frame.slots.end()) {
                    return {BackendErrorCode::INTERNAL_ERROR,
                            "missing stack slot for binary result"};
                }

                auto lhs_result = emit_value_to_freg(binary.lhs, "ft0", ctx, out);
                if (!lhs_result.ok()) {
                    return lhs_result;
                }
                auto rhs_result = emit_value_to_freg(binary.rhs, "ft1", ctx, out);
                if (!rhs_result.ok()) {
                    return rhs_result;
                }

                const char* suffix = detail::float_suffix(binary.lhs->ty);
                if (!suffix) {
                    return {BackendErrorCode::UNSUPPORTED_TYPE,
                            "floating-point binary operands must be FLOAT or DOUBLE"};
                }

                switch (binary.op) {
                    case ir::BinaryOp::ADD:
                        if (!detail::is_float_scalar_type(inst->ty) ||
                            inst->ty->tag != binary.lhs->ty->tag) {
                            return {BackendErrorCode::INVALID_PROGRAM,
                                    "floating-point arithmetic result type must match operand type"};
                        }
                        out << "  fadd." << suffix << " ft2, ft0, ft1\n";
                        return emit_store_freg_to_slot("ft2", slot_it->second, inst->ty, out);
                    case ir::BinaryOp::SUB:
                        if (!detail::is_float_scalar_type(inst->ty) ||
                            inst->ty->tag != binary.lhs->ty->tag) {
                            return {BackendErrorCode::INVALID_PROGRAM,
                                    "floating-point arithmetic result type must match operand type"};
                        }
                        out << "  fsub." << suffix << " ft2, ft0, ft1\n";
                        return emit_store_freg_to_slot("ft2", slot_it->second, inst->ty, out);
                    case ir::BinaryOp::MUL:
                        if (!detail::is_float_scalar_type(inst->ty) ||
                            inst->ty->tag != binary.lhs->ty->tag) {
                            return {BackendErrorCode::INVALID_PROGRAM,
                                    "floating-point arithmetic result type must match operand type"};
                        }
                        out << "  fmul." << suffix << " ft2, ft0, ft1\n";
                        return emit_store_freg_to_slot("ft2", slot_it->second, inst->ty, out);
                    case ir::BinaryOp::DIV:
                        if (!detail::is_float_scalar_type(inst->ty) ||
                            inst->ty->tag != binary.lhs->ty->tag) {
                            return {BackendErrorCode::INVALID_PROGRAM,
                                    "floating-point arithmetic result type must match operand type"};
                        }
                        out << "  fdiv." << suffix << " ft2, ft0, ft1\n";
                        return emit_store_freg_to_slot("ft2", slot_it->second, inst->ty, out);
                    case ir::BinaryOp::EQ:
                        if (!detail::is_integer_scalar_type(inst->ty)) {
                            return {BackendErrorCode::INVALID_PROGRAM,
                                    "floating-point comparison result type must be an integer scalar"};
                        }
                        out << "  feq." << suffix << " t2, ft0, ft1\n";
                        return emit_store_reg_to_slot("t2", slot_it->second, inst->ty, out);
                    case ir::BinaryOp::NOT_EQ:
                        if (!detail::is_integer_scalar_type(inst->ty)) {
                            return {BackendErrorCode::INVALID_PROGRAM,
                                    "floating-point comparison result type must be an integer scalar"};
                        }
                        out << "  feq." << suffix << " t2, ft0, ft1\n";
                        out << "  seqz t2, t2\n";
                        return emit_store_reg_to_slot("t2", slot_it->second, inst->ty, out);
                    case ir::BinaryOp::LT:
                        if (!detail::is_integer_scalar_type(inst->ty)) {
                            return {BackendErrorCode::INVALID_PROGRAM,
                                    "floating-point comparison result type must be an integer scalar"};
                        }
                        out << "  flt." << suffix << " t2, ft0, ft1\n";
                        return emit_store_reg_to_slot("t2", slot_it->second, inst->ty, out);
                    case ir::BinaryOp::LE:
                        if (!detail::is_integer_scalar_type(inst->ty)) {
                            return {BackendErrorCode::INVALID_PROGRAM,
                                    "floating-point comparison result type must be an integer scalar"};
                        }
                        out << "  fle." << suffix << " t2, ft0, ft1\n";
                        return emit_store_reg_to_slot("t2", slot_it->second, inst->ty, out);
                    case ir::BinaryOp::GT:
                        if (!detail::is_integer_scalar_type(inst->ty)) {
                            return {BackendErrorCode::INVALID_PROGRAM,
                                    "floating-point comparison result type must be an integer scalar"};
                        }
                        out << "  flt." << suffix << " t2, ft1, ft0\n";
                        return emit_store_reg_to_slot("t2", slot_it->second, inst->ty, out);
                    case ir::BinaryOp::GE:
                        if (!detail::is_integer_scalar_type(inst->ty)) {
                            return {BackendErrorCode::INVALID_PROGRAM,
                                    "floating-point comparison result type must be an integer scalar"};
                        }
                        out << "  fle." << suffix << " t2, ft1, ft0\n";
                        return emit_store_reg_to_slot("t2", slot_it->second, inst->ty, out);
                    case ir::BinaryOp::MOD:
                    case ir::BinaryOp::AND:
                    case ir::BinaryOp::OR:
                    case ir::BinaryOp::XOR:
                    case ir::BinaryOp::SHL:
                    case ir::BinaryOp::SHR:
                    case ir::BinaryOp::SAR:
                        return {BackendErrorCode::UNSUPPORTED_TYPE,
                                "floating-point binary op is not supported by the RV64 backend in stage 4"};
                }
            }

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
                    if (is_i32) {
                        out << "  addiw t2, t2, 0\n";
                    }
                    break;
                case ir::BinaryOp::OR:
                    out << "  or t2, t0, t1\n";
                    if (is_i32) {
                        out << "  addiw t2, t2, 0\n";
                    }
                    break;
                case ir::BinaryOp::XOR:
                    out << "  xor t2, t0, t1\n";
                    if (is_i32) {
                        out << "  addiw t2, t2, 0\n";
                    }
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
        case ir::ValueTag::CALL: {
            const auto& call = std::get<ir::Call>(inst->kind.data);
            std::vector<ir::Type> arg_types;
            std::vector<ir::Value> args;
            bool has_float_without_prototype = false;
            for (const auto& arg_item : call.args.buffer) {
                if (!std::holds_alternative<ir::Value>(arg_item)) {
                    continue;
                }
                auto arg = std::get<ir::Value>(arg_item);
                args.push_back(arg);
                has_float_without_prototype =
                    has_float_without_prototype ||
                    detail::is_float_scalar_type(arg ? arg->ty : nullptr);
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
                    if (arg_types.size() != args.size()) {
                        return {BackendErrorCode::UNSUPPORTED_IR,
                                "variadic or mismatched function calls are unsupported"};
                    }
                } else if (has_float_without_prototype && !args.empty()) {
                    return {BackendErrorCode::UNSUPPORTED_IR,
                            "floating-point calls without a visible prototype are unsupported"};
                }
            }

            if (arg_types.empty()) {
                for (const auto& arg : args) {
                    arg_types.push_back(arg ? arg->ty : nullptr);
                }
            }

            detail::CallArgLayout arg_layout;
            auto classify_result = detail::classify_call_arguments(arg_types, arg_layout);
            if (!classify_result.ok()) {
                return classify_result;
            }

            for (std::size_t i = 0; i < args.size(); ++i) {
                auto arg = args[i];
                const auto& loc = arg_layout.locations[i];
                BackendError arg_result = BackendError::success();
                switch (loc.kind) {
                    case RISCV64ArgLocation::Kind::IntReg:
                        if (detail::is_float_scalar_type(arg ? arg->ty : nullptr)) {
                            arg_result = emit_value_to_freg(arg, "ft0", ctx, out);
                            if (arg_result.ok()) {
                                if (arg->ty->tag == ir::TypeTag::FLOAT) {
                                    out << "  fmv.x.w t0, ft0\n";
                                } else {
                                    out << "  fmv.x.d t0, ft0\n";
                                }
                                out << "  mv a" << loc.index << ", t0\n";
                            }
                        } else {
                            arg_result = emit_value_to_reg(arg, "t0", ctx, out);
                            if (arg_result.ok()) {
                                out << "  mv a" << loc.index << ", t0\n";
                            }
                        }
                        break;
                    case RISCV64ArgLocation::Kind::FloatReg:
                        arg_result = emit_value_to_freg(arg, "ft0", ctx, out);
                        if (arg_result.ok()) {
                            const char* suffix = detail::float_move_suffix(arg ? arg->ty : nullptr);
                            if (!suffix) {
                                return {BackendErrorCode::UNSUPPORTED_TYPE,
                                        "floating-point argument must be FLOAT or DOUBLE"};
                            }
                            out << "  fsgnj." << suffix << " fa" << loc.index
                                << ", ft0, ft0\n";
                        }
                        break;
                    case RISCV64ArgLocation::Kind::Stack:
                        if (loc.stack_offset != 0) {
                            out << "  addi t1, sp, " << loc.stack_offset << '\n';
                        }
                        {
                            const char* addr_reg = loc.stack_offset == 0 ? "sp" : "t1";
                            if (detail::is_float_scalar_type(arg ? arg->ty : nullptr)) {
                                arg_result = emit_value_to_freg(arg, "ft0", ctx, out);
                                if (arg_result.ok()) {
                                    arg_result = emit_store_freg_to_address(
                                        "ft0", addr_reg, arg ? arg->ty : nullptr, out);
                                }
                            } else {
                                arg_result = emit_value_to_reg(arg, "t0", ctx, out);
                                if (arg_result.ok()) {
                                    arg_result = emit_store_to_address(
                                        "t0", addr_reg, arg ? arg->ty : nullptr, out);
                                }
                            }
                        }
                        break;
                }
                if (!arg_result.ok()) {
                    return arg_result;
                }
            }

            out << "  call " << function_symbol(call.callee) << '\n';
            auto slot_it = ctx.frame.slots.find(inst.get());
            if (slot_it != ctx.frame.slots.end() && inst->ty && inst->ty->tag != ir::TypeTag::UNIT) {
                if (detail::is_float_scalar_type(inst->ty)) {
                    return emit_store_freg_to_slot("fa0", slot_it->second, inst->ty, out);
                }
                return emit_store_reg_to_slot("a0", slot_it->second, inst->ty, out);
            }
            return BackendError::success();
        }
        case ir::ValueTag::GET_PTR:
        case ir::ValueTag::GET_ELEM_PTR: {
            auto addr_result = emit_address_to_reg(inst, "t0", ctx, out);
            if (!addr_result.ok()) {
                return addr_result;
            }
            auto slot_it = ctx.frame.slots.find(inst.get());
            if (slot_it == ctx.frame.slots.end()) {
                return {BackendErrorCode::INTERNAL_ERROR,
                        "missing stack slot for pointer arithmetic result"};
            }
            ir::Type result_type = inst->ty;
            if (!result_type || result_type->tag != ir::TypeTag::POINTER) {
                return {BackendErrorCode::INVALID_PROGRAM,
                        "pointer arithmetic result type is missing or not a pointer"};
            }
            return emit_store_reg_to_slot("t0", slot_it->second, result_type, out);
        }
        case ir::ValueTag::CAST: {
            const auto& cast = std::get<ir::Cast>(inst->kind.data);
            const auto classification = detail::classify_cast(inst);
            auto validation_result = detail::validate_cast_operands(inst, cast, classification);
            if (!validation_result.ok()) {
                return validation_result;
            }

            auto slot_it = ctx.frame.slots.find(inst.get());
            if (slot_it == ctx.frame.slots.end()) {
                return {BackendErrorCode::INTERNAL_ERROR, "cast result has no stack slot"};
            }

            switch (classification) {
                case detail::CastClassification::SITOFP: {
                    auto load_result = emit_value_to_reg(cast.value, "t0", ctx, out);
                    if (!load_result.ok()) {
                        return load_result;
                    }
                    const char* dst_suffix = detail::float_suffix(inst->ty);
                    if (!dst_suffix) {
                        return {BackendErrorCode::INVALID_PROGRAM,
                                "sitofp target type must be FLOAT or DOUBLE"};
                    }
                    const char* src_suffix =
                        cast.value->ty && cast.value->ty->tag == ir::TypeTag::INT64 ? "l" : "w";
                    out << "  fcvt." << dst_suffix << "." << src_suffix << " ft0, t0\n";
                    return emit_store_freg_to_slot("ft0", slot_it->second, inst->ty, out);
                }
                case detail::CastClassification::FPTOSI: {
                    auto load_result = emit_value_to_freg(cast.value, "ft0", ctx, out);
                    if (!load_result.ok()) {
                        return load_result;
                    }
                    const char* src_suffix = detail::float_suffix(cast.value->ty);
                    if (!src_suffix) {
                        return {BackendErrorCode::INVALID_PROGRAM,
                                "fptosi source type must be FLOAT or DOUBLE"};
                    }
                    const char* dst_suffix =
                        inst->ty && inst->ty->tag == ir::TypeTag::INT64 ? "l" : "w";
                    out << "  fcvt." << dst_suffix << "." << src_suffix << " t0, ft0\n";
                    return emit_store_reg_to_slot("t0", slot_it->second, inst->ty, out);
                }
            }

            return {BackendErrorCode::INVALID_PROGRAM,
                    "cast operation classification is invalid"};
        }
        default:
            return {BackendErrorCode::UNSUPPORTED_IR,
                    "encountered unsupported value kind while emitting RV64 assembly"};
    }
}

} // namespace rcc::backend
