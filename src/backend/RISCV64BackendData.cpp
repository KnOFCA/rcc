// Author: KnOFCA

#include "backend/RISCV64BackendInternal.h"

namespace rcc::backend {

namespace {

std::string format_symbol_with_offset(const std::string& symbol, std::int64_t offset) {
    if (offset == 0) {
        return symbol;
    }
    if (offset > 0) {
        return symbol + " + " + std::to_string(offset);
    }
    return symbol + " - " + std::to_string(-offset);
}

} // namespace

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

        const auto* arr = detail::as_array(type);
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
    switch (type->tag) {
        case ir::TypeTag::INT8:
            if (!integer.has_value()) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "global initializer is not a supported integer constant"};
            }
            out << "  .byte " << *integer << '\n';
            return BackendError::success();
        case ir::TypeTag::INT16:
            if (!integer.has_value()) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "global initializer is not a supported integer constant"};
            }
            out << "  .half " << *integer << '\n';
            return BackendError::success();
        case ir::TypeTag::INT32:
            if (!integer.has_value()) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "global initializer is not a supported integer constant"};
            }
            out << "  .word " << *integer << '\n';
            return BackendError::success();
        case ir::TypeTag::INT64:
            if (!integer.has_value()) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "global initializer is not a supported integer constant"};
            }
            out << "  .dword " << *integer << '\n';
            return BackendError::success();
        case ir::TypeTag::POINTER: {
            if (integer.has_value()) {
                out << "  .dword " << *integer << '\n';
                return BackendError::success();
            }
            auto address = extract_global_address(init);
            if (!address.has_value()) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "global initializer is not a supported pointer constant"};
            }
            out << "  .dword " << format_symbol_with_offset(address->first, address->second)
                << '\n';
            return BackendError::success();
        }
        case ir::TypeTag::FLOAT: {
            auto bits = extract_float_bits(init);
            if (!bits.has_value()) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "global initializer is not a supported float constant"};
            }
            out << "  .word " << *bits << '\n';
            return BackendError::success();
        }
        case ir::TypeTag::DOUBLE: {
            auto bits = extract_double_bits(init);
            if (!bits.has_value()) {
                return {BackendErrorCode::UNSUPPORTED_IR,
                        "global initializer is not a supported double constant"};
            }
            out << "  .dword " << *bits << '\n';
            return BackendError::success();
        }
        default:
            return {BackendErrorCode::UNSUPPORTED_TYPE,
                    "global initializer type is not supported by the RV64 scaffold backend"};
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

std::optional<std::pair<std::string, std::int64_t>>
RISCV64Backend::extract_global_address(const ir::Value& value) const {
    if (!value) {
        return std::nullopt;
    }

    if (value->kind.tag == ir::ValueTag::GLOBAL_ALLOC) {
        return std::pair<std::string, std::int64_t>{value_symbol(value), 0};
    }

    auto extract_index_offset = [&](const ir::Value& base,
                                    const ir::Value& index,
                                    ir::Type step_type) -> std::optional<std::pair<std::string, std::int64_t>> {
        auto base_addr = extract_global_address(base);
        auto idx = extract_integer(index);
        const auto step = static_cast<std::int64_t>(type_size(step_type));
        if (!base_addr.has_value() || !idx.has_value() || step == 0) {
            return std::nullopt;
        }
        base_addr->second += (*idx) * step;
        return base_addr;
    };

    if (value->kind.tag == ir::ValueTag::GET_ELEM_PTR) {
        const auto& gep = std::get<ir::GetElemPtr>(value->kind.data);
        return extract_index_offset(gep.src, gep.index, get_elem_ptr_step_type(gep.src));
    }

    if (value->kind.tag == ir::ValueTag::GET_PTR) {
        const auto& gp = std::get<ir::GetPtr>(value->kind.data);
        return extract_index_offset(gp.src, gp.index, get_ptr_step_type(gp.src));
    }

    return std::nullopt;
}

std::optional<std::uint32_t> RISCV64Backend::extract_float_bits(const ir::Value& value) const {
    if (!value || value->kind.tag != ir::ValueTag::NUMBER) {
        return std::nullopt;
    }

    const auto& number = std::get<ir::Number>(value->kind.data);
    if (!std::holds_alternative<ir::Float>(number.num)) {
        return std::nullopt;
    }
    return std::bit_cast<std::uint32_t>(std::get<ir::Float>(number.num).value);
}

std::optional<std::uint64_t> RISCV64Backend::extract_double_bits(const ir::Value& value) const {
    if (!value || value->kind.tag != ir::ValueTag::NUMBER) {
        return std::nullopt;
    }

    const auto& number = std::get<ir::Number>(value->kind.data);
    if (!std::holds_alternative<ir::Double>(number.num)) {
        return std::nullopt;
    }
    return std::bit_cast<std::uint64_t>(std::get<ir::Double>(number.num).value);
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
            const auto* arr = detail::as_array(type);
            return arr ? type_size(arr->base) * arr->len : 0;
        }
        case ir::TypeTag::UNIT: return 0;
        case ir::TypeTag::FLOAT: return 4;
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
            const auto* arr = detail::as_array(type);
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

} // namespace rcc::backend