// Author: KnOFCA

#include "backend/RISCV64BackendInternal.h"

namespace rcc::backend {

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
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "floating-point scalar values must be materialized into floating-point registers"};
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
            if (ref.index >= ctx.arg_locations.size()) {
                return {BackendErrorCode::INVALID_PROGRAM, "function argument index is out of range"};
            }
            return emit_arg_from_location_to_reg(
                ctx.arg_locations[ref.index], reg, ctx.frame.stack_size, out);
        }
        case ir::ValueTag::LOAD:
        case ir::ValueTag::BINARY:
        case ir::ValueTag::CALL:
        case ir::ValueTag::CAST: {
            if (detail::is_float_scalar_type(value->ty)) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "floating-point scalar values must be materialized into floating-point registers"};
            }
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

BackendError RISCV64Backend::emit_value_to_freg(const ir::Value& value,
                                                const std::string& freg,
                                                FunctionContext& ctx,
                                                std::ostream& out) {
    if (!value) {
        return {BackendErrorCode::INVALID_PROGRAM, "attempted to materialize null value"};
    }
    if (!detail::is_float_scalar_type(value->ty)) {
        return {BackendErrorCode::UNSUPPORTED_TYPE,
                "value is not a FLOAT/DOUBLE scalar for floating-point register materialization"};
    }

    switch (value->kind.tag) {
        case ir::ValueTag::NUMBER:
            if (value->ty->tag == ir::TypeTag::FLOAT) {
                auto bits = extract_float_bits(value);
                if (!bits.has_value()) {
                    return {BackendErrorCode::UNSUPPORTED_IR,
                            "failed to extract float bit pattern"};
                }
                out << "  li t0, " << *bits << '\n';
                out << "  fmv.w.x " << freg << ", t0\n";
                return BackendError::success();
            }
            if (value->ty->tag == ir::TypeTag::DOUBLE) {
                auto bits = extract_double_bits(value);
                if (!bits.has_value()) {
                    return {BackendErrorCode::UNSUPPORTED_IR,
                            "failed to extract double bit pattern"};
                }
                out << "  li t0, " << *bits << '\n';
                out << "  fmv.d.x " << freg << ", t0\n";
                return BackendError::success();
            }
            break;
        case ir::ValueTag::LOAD:
        case ir::ValueTag::BINARY:
        case ir::ValueTag::CAST: {
            auto slot_it = ctx.frame.slots.find(value.get());
            if (slot_it == ctx.frame.slots.end()) {
                return {BackendErrorCode::INTERNAL_ERROR, "missing stack slot for computed value"};
            }
            return emit_load_slot_to_freg(slot_it->second, value->ty, freg, out);
        }
        case ir::ValueTag::FUNC_ARG_REF: {
            const auto& ref = std::get<ir::FuncArgRef>(value->kind.data);
            if (ref.index >= ctx.arg_locations.size()) {
                return {BackendErrorCode::INVALID_PROGRAM, "function argument index is out of range"};
            }
            return emit_arg_from_location_to_freg(
                ctx.arg_locations[ref.index], freg, ctx.frame.stack_size, out);
        }
        case ir::ValueTag::CALL: {
            auto slot_it = ctx.frame.slots.find(value.get());
            if (slot_it == ctx.frame.slots.end()) {
                return {BackendErrorCode::INTERNAL_ERROR, "missing stack slot for computed value"};
            }
            return emit_load_slot_to_freg(slot_it->second, value->ty, freg, out);
        }
        default:
            break;
    }

    return {BackendErrorCode::UNSUPPORTED_IR,
            "value kind cannot be materialized as a floating-point register yet"};
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
        case ir::ValueTag::LOAD:
        case ir::ValueTag::CALL:
        case ir::ValueTag::FUNC_ARG_REF:
            if (!value->ty || value->ty->tag != ir::TypeTag::POINTER) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "value kind cannot be materialized as an address"};
            }
            return emit_value_to_reg(value, reg, ctx, out);
        case ir::ValueTag::GET_PTR: {
            const auto& get_ptr = std::get<ir::GetPtr>(value->kind.data);
            if (!get_ptr.src || !get_ptr.src->ty || get_ptr.src->ty->tag != ir::TypeTag::POINTER) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "GET_PTR source is not a pointer-like value"};
            }
            const auto step_type = get_ptr_step_type(get_ptr.src);
            if (!step_type) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "GET_PTR step type cannot be derived"};
            }
            const auto step_size = type_size(step_type);
            if (step_size == 0) {
                return {BackendErrorCode::UNSUPPORTED_TYPE,
                        "GET_PTR step type has unsupported size"};
            }

            BackendError base_result = detail::is_address_expression(get_ptr.src)
                                           ? emit_address_to_reg(get_ptr.src, reg, ctx, out)
                                           : emit_value_to_reg(get_ptr.src, reg, ctx, out);
            if (!base_result.ok()) {
                return base_result;
            }
            auto index_result = emit_scaled_index_to_reg(get_ptr.index, step_size, "t3", ctx, out);
            if (!index_result.ok()) {
                return index_result;
            }
            out << "  add " << reg << ", " << reg << ", t3\n";
            return BackendError::success();
        }
        case ir::ValueTag::GET_ELEM_PTR: {
            const auto& get_elem_ptr = std::get<ir::GetElemPtr>(value->kind.data);
            if (!get_elem_ptr.src || !get_elem_ptr.src->ty ||
                get_elem_ptr.src->ty->tag != ir::TypeTag::POINTER) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "GET_ELEM_PTR source is not pointer-to-array"};
            }
            const auto step_type = get_elem_ptr_step_type(get_elem_ptr.src);
            if (!step_type) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "GET_ELEM_PTR source is not pointer-to-array"};
            }
            const auto step_size = type_size(step_type);
            if (step_size == 0) {
                return {BackendErrorCode::UNSUPPORTED_TYPE,
                        "GET_ELEM_PTR step type has unsupported size"};
            }

            BackendError base_result = detail::is_address_expression(get_elem_ptr.src)
                                           ? emit_address_to_reg(get_elem_ptr.src, reg, ctx, out)
                                           : emit_value_to_reg(get_elem_ptr.src, reg, ctx, out);
            if (!base_result.ok()) {
                return base_result;
            }
            auto index_result =
                emit_scaled_index_to_reg(get_elem_ptr.index, step_size, "t3", ctx, out);
            if (!index_result.ok()) {
                return index_result;
            }
            out << "  add " << reg << ", " << reg << ", t3\n";
            return BackendError::success();
        }
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

BackendError RISCV64Backend::emit_load_from_address_to_freg(const std::string& addr_reg,
                                                            ir::Type type,
                                                            const std::string& dst_freg,
                                                            std::ostream& out) {
    if (!type) {
        return {BackendErrorCode::INVALID_PROGRAM, "load result type is missing"};
    }
    switch (type->tag) {
        case ir::TypeTag::FLOAT:
            out << "  flw " << dst_freg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        case ir::TypeTag::DOUBLE:
            out << "  fld " << dst_freg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        default:
            return {BackendErrorCode::UNSUPPORTED_TYPE,
                    "floating-point load type is not supported by RV64 backend"};
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

BackendError RISCV64Backend::emit_store_freg_to_address(const std::string& src_freg,
                                                        const std::string& addr_reg,
                                                        ir::Type type,
                                                        std::ostream& out) {
    if (!type) {
        return {BackendErrorCode::INVALID_PROGRAM, "store value type is missing"};
    }
    switch (type->tag) {
        case ir::TypeTag::FLOAT:
            out << "  fsw " << src_freg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        case ir::TypeTag::DOUBLE:
            out << "  fsd " << src_freg << ", 0(" << addr_reg << ")\n";
            return BackendError::success();
        default:
            return {BackendErrorCode::UNSUPPORTED_TYPE,
                    "floating-point store type is not supported by RV64 backend"};
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

BackendError RISCV64Backend::emit_store_freg_to_slot(const std::string& src_freg,
                                                     const StackSlot& slot,
                                                     ir::Type type,
                                                     std::ostream& out) {
    if (!type) {
        return {BackendErrorCode::INVALID_PROGRAM, "stack store type is missing"};
    }
    switch (type->tag) {
        case ir::TypeTag::FLOAT:
            out << "  fsw " << src_freg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        case ir::TypeTag::DOUBLE:
            out << "  fsd " << src_freg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        case ir::TypeTag::UNIT:
            return BackendError::success();
        default:
            return {BackendErrorCode::UNSUPPORTED_TYPE, "floating-point stack slot type is not supported"};
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

BackendError RISCV64Backend::emit_load_slot_to_freg(const StackSlot& slot,
                                                    ir::Type type,
                                                    const std::string& dst_freg,
                                                    std::ostream& out) {
    if (!type) {
        return {BackendErrorCode::INVALID_PROGRAM, "stack load type is missing"};
    }
    switch (type->tag) {
        case ir::TypeTag::FLOAT:
            out << "  flw " << dst_freg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        case ir::TypeTag::DOUBLE:
            out << "  fld " << dst_freg << ", " << slot.offset << "(sp)\n";
            return BackendError::success();
        default:
            return {BackendErrorCode::UNSUPPORTED_TYPE, "floating-point stack load type is not supported"};
    }
}

ir::Type RISCV64Backend::pointee_type(ir::Type ptr_ty) const {
    const auto* ptr = detail::as_pointer(ptr_ty);
    return ptr ? ptr->base : nullptr;
}

ir::Type RISCV64Backend::get_ptr_step_type(const ir::Value& src) const {
    const auto base_type = pointee_type(src ? src->ty : nullptr);
    if (!base_type) {
        return nullptr;
    }
    const auto* arr = detail::as_array(base_type);
    if (arr && detail::is_address_expression(src)) {
        return arr->base;
    }
    return base_type;
}

ir::Type RISCV64Backend::get_elem_ptr_step_type(const ir::Value& src) const {
    const auto base_type = pointee_type(src ? src->ty : nullptr);
    const auto* arr = detail::as_array(base_type);
    if (!arr) {
        return nullptr;
    }
    return arr->base;
}

BackendError RISCV64Backend::emit_scaled_index_to_reg(const ir::Value& index,
                                                      std::size_t step_size,
                                                      const std::string& dst_reg,
                                                      FunctionContext& ctx,
                                                      std::ostream& out) {
    if (!index || !detail::is_integer_scalar_type(index->ty)) {
        return {BackendErrorCode::UNSUPPORTED_IR, "pointer index is not an integer scalar"};
    }

    auto index_result = emit_value_to_reg(index, dst_reg, ctx, out);
    if (!index_result.ok()) {
        return index_result;
    }

    if (step_size == 1) {
        return BackendError::success();
    }

    out << "  li t4, " << step_size << '\n';
    out << "  mul " << dst_reg << ", " << dst_reg << ", t4\n";
    return BackendError::success();
}

BackendError RISCV64Backend::emit_arg_from_location_to_reg(const RISCV64ArgLocation& loc,
                                                           const std::string& reg,
                                                           std::size_t stack_base_offset,
                                                           std::ostream& out) {
    switch (loc.kind) {
        case RISCV64ArgLocation::Kind::IntReg:
            out << "  mv " << reg << ", a" << loc.index << '\n';
            return BackendError::success();
        case RISCV64ArgLocation::Kind::Stack:
            switch (loc.type ? loc.type->tag : ir::TypeTag::UNIT) {
                case ir::TypeTag::INT8:
                    out << "  lb " << reg << ", " << (stack_base_offset + loc.stack_offset)
                        << "(sp)\n";
                    return BackendError::success();
                case ir::TypeTag::INT16:
                    out << "  lh " << reg << ", " << (stack_base_offset + loc.stack_offset)
                        << "(sp)\n";
                    return BackendError::success();
                case ir::TypeTag::INT32:
                    out << "  lw " << reg << ", " << (stack_base_offset + loc.stack_offset)
                        << "(sp)\n";
                    return BackendError::success();
                case ir::TypeTag::INT64:
                case ir::TypeTag::POINTER:
                    out << "  ld " << reg << ", " << (stack_base_offset + loc.stack_offset)
                        << "(sp)\n";
                    return BackendError::success();
                case ir::TypeTag::FLOAT:
                    out << "  flw ft5, " << (stack_base_offset + loc.stack_offset) << "(sp)\n";
                    out << "  fmv.x.w " << reg << ", ft5\n";
                    return BackendError::success();
                case ir::TypeTag::DOUBLE:
                    out << "  fld ft5, " << (stack_base_offset + loc.stack_offset) << "(sp)\n";
                    out << "  fmv.x.d " << reg << ", ft5\n";
                    return BackendError::success();
                default:
                    return {BackendErrorCode::UNSUPPORTED_TYPE,
                            "unsupported stack-passed argument type"};
            }
        case RISCV64ArgLocation::Kind::FloatReg:
            if (!detail::is_float_scalar_type(loc.type)) {
                return {BackendErrorCode::INVALID_PROGRAM,
                        "floating-point argument register used for non-floating type"};
            }
            if (loc.type->tag == ir::TypeTag::FLOAT) {
                out << "  fmv.x.w " << reg << ", fa" << loc.index << '\n';
                return BackendError::success();
            }
            if (loc.type->tag == ir::TypeTag::DOUBLE) {
                out << "  fmv.x.d " << reg << ", fa" << loc.index << '\n';
                return BackendError::success();
            }
            return {BackendErrorCode::UNSUPPORTED_TYPE, "unsupported floating-point argument type"};
    }

    return {BackendErrorCode::INTERNAL_ERROR, "invalid argument location kind"};
}

BackendError RISCV64Backend::emit_arg_from_location_to_freg(const RISCV64ArgLocation& loc,
                                                            const std::string& freg,
                                                            std::size_t stack_base_offset,
                                                            std::ostream& out) {
    if (!detail::is_float_scalar_type(loc.type)) {
        return {BackendErrorCode::UNSUPPORTED_TYPE,
                "attempted to load non-floating argument into a floating-point register"};
    }

    const char* suffix = detail::float_move_suffix(loc.type);
    if (!suffix) {
        return {BackendErrorCode::UNSUPPORTED_TYPE,
                "floating-point argument must be FLOAT or DOUBLE"};
    }

    switch (loc.kind) {
        case RISCV64ArgLocation::Kind::FloatReg:
            out << "  fsgnj." << suffix << " " << freg << ", fa" << loc.index
                << ", fa" << loc.index << '\n';
            return BackendError::success();
        case RISCV64ArgLocation::Kind::IntReg:
            if (loc.type->tag == ir::TypeTag::FLOAT) {
                out << "  fmv.w.x " << freg << ", a" << loc.index << '\n';
                return BackendError::success();
            }
            out << "  fmv.d.x " << freg << ", a" << loc.index << '\n';
            return BackendError::success();
        case RISCV64ArgLocation::Kind::Stack:
            if (loc.type->tag == ir::TypeTag::FLOAT) {
                out << "  flw " << freg << ", " << (stack_base_offset + loc.stack_offset)
                    << "(sp)\n";
                return BackendError::success();
            }
            out << "  fld " << freg << ", " << (stack_base_offset + loc.stack_offset)
                << "(sp)\n";
            return BackendError::success();
    }

    return {BackendErrorCode::INTERNAL_ERROR, "invalid argument location kind"};
}

} // namespace rcc::backend