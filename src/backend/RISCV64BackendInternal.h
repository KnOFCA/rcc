// Author: KnOFCA

#pragma once

#include "backend/RISCV64Backend.h"

#include <bit>
#include <memory>
#include <variant>

namespace rcc::backend::detail {

inline std::size_t align_to(std::size_t value, std::size_t align) {
    if (align == 0) {
        return value;
    }
    const std::size_t remainder = value % align;
    return remainder == 0 ? value : value + (align - remainder);
}

inline std::string strip_global_prefix(const std::optional<std::string>& name,
                                       const std::string& fallback) {
    if (!name || name->empty()) {
        return fallback;
    }
    if ((*name)[0] == '@' || (*name)[0] == '%') {
        return name->substr(1);
    }
    return *name;
}

inline const ir::TypeKind::pointer* as_pointer(ir::Type type) {
    if (!type || type->tag != ir::TypeTag::POINTER) {
        return nullptr;
    }
    auto ptr = std::get<std::shared_ptr<ir::TypeKind::pointer>>(type->data);
    return ptr.get();
}

inline const ir::TypeKind::array* as_array(ir::Type type) {
    if (!type || type->tag != ir::TypeTag::ARRAY) {
        return nullptr;
    }
    auto arr = std::get<std::shared_ptr<ir::TypeKind::array>>(type->data);
    return arr.get();
}

inline bool has_result(const ir::Value& value) {
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

inline bool is_integer_scalar_type(ir::Type type) {
    if (!type) {
        return false;
    }
    switch (type->tag) {
        case ir::TypeTag::INT8:
        case ir::TypeTag::INT16:
        case ir::TypeTag::INT32:
        case ir::TypeTag::INT64:
            return true;
        default:
            return false;
    }
}

inline bool is_float_scalar_type(ir::Type type) {
    if (!type) {
        return false;
    }
    return type->tag == ir::TypeTag::FLOAT || type->tag == ir::TypeTag::DOUBLE;
}

inline bool is_float_binary_instruction(const ir::Value& inst) {
    if (!inst || inst->kind.tag != ir::ValueTag::BINARY) {
        return false;
    }
    const auto& binary = std::get<ir::Binary>(inst->kind.data);
    return is_float_scalar_type(binary.lhs ? binary.lhs->ty : nullptr) ||
           is_float_scalar_type(binary.rhs ? binary.rhs->ty : nullptr);
}

inline const char* float_suffix(ir::Type type) {
    if (!type) {
        return nullptr;
    }
    switch (type->tag) {
        case ir::TypeTag::FLOAT:
            return "s";
        case ir::TypeTag::DOUBLE:
            return "d";
        default:
            return nullptr;
    }
}

enum class CastClassification {
    SITOFP,
    FPTOSI,
    INTCAST,
    FPEXT,
    FPTRUNC,
};

inline CastClassification classify_cast(const ir::Value& inst) {
    const auto& cast = std::get<ir::Cast>(inst->kind.data);
    switch (cast.op) {
        case ir::CastOp::SITOFP:
            return CastClassification::SITOFP;
        case ir::CastOp::FPTOSI:
            return CastClassification::FPTOSI;
        case ir::CastOp::INTCAST:
            return CastClassification::INTCAST;
        case ir::CastOp::FPEXT:
            return CastClassification::FPEXT;
        case ir::CastOp::FPTRUNC:
            return CastClassification::FPTRUNC;
    }

    return CastClassification::SITOFP;
}

inline BackendError validate_cast_operands(const ir::Value& inst,
                                           const ir::Cast& cast,
                                           CastClassification classification) {
    if (!cast.value) {
        return {BackendErrorCode::INVALID_PROGRAM, "cast source value is missing"};
    }
    if (!inst->ty) {
        return {BackendErrorCode::INVALID_PROGRAM, "cast result type is missing"};
    }
    if (!cast.value->ty) {
        return {BackendErrorCode::INVALID_PROGRAM, "cast source type is missing"};
    }

    switch (classification) {
        case CastClassification::SITOFP:
            if (!is_integer_scalar_type(cast.value->ty)) {
                return {BackendErrorCode::UNSUPPORTED_TYPE,
                        "sitofp source type is not a supported integer scalar"};
            }
            if (!is_float_scalar_type(inst->ty)) {
                return {BackendErrorCode::INVALID_PROGRAM,
                        "sitofp target type must be FLOAT or DOUBLE"};
            }
            return BackendError::success();
        case CastClassification::FPTOSI:
            if (!is_float_scalar_type(cast.value->ty)) {
                return {BackendErrorCode::UNSUPPORTED_TYPE,
                        "fptosi source type is not FLOAT or DOUBLE"};
            }
            if (!is_integer_scalar_type(inst->ty)) {
                return {BackendErrorCode::INVALID_PROGRAM,
                        "fptosi target type is not a supported integer scalar"};
            }
            return BackendError::success();
        case CastClassification::INTCAST:
            if (!is_integer_scalar_type(cast.value->ty)) {
                return {BackendErrorCode::UNSUPPORTED_TYPE,
                        "intcast source type is not a supported integer scalar"};
            }
            if (!is_integer_scalar_type(inst->ty)) {
                return {BackendErrorCode::INVALID_PROGRAM,
                        "intcast target type is not a supported integer scalar"};
            }
            return BackendError::success();
        case CastClassification::FPEXT:
        case CastClassification::FPTRUNC:
            if (!is_float_scalar_type(cast.value->ty)) {
                return {BackendErrorCode::UNSUPPORTED_TYPE,
                        "float cast source type is not FLOAT or DOUBLE"};
            }
            if (!is_float_scalar_type(inst->ty)) {
                return {BackendErrorCode::INVALID_PROGRAM,
                        "float cast target type must be FLOAT or DOUBLE"};
            }
            return BackendError::success();
    }

    return {BackendErrorCode::INVALID_PROGRAM, "cast operation classification is invalid"};
}

inline bool is_address_expression(const ir::Value& value) {
    if (!value) {
        return false;
    }
    switch (value->kind.tag) {
        case ir::ValueTag::ALLOC:
        case ir::ValueTag::GLOBAL_ALLOC:
        case ir::ValueTag::GET_PTR:
        case ir::ValueTag::GET_ELEM_PTR:
            return true;
        default:
            return false;
    }
}

struct CallArgLayout {
    std::vector<RISCV64ArgLocation> locations;
    std::size_t stack_arg_size{0};
};

inline bool is_integer_abi_scalar_type(ir::Type type) {
    return is_integer_scalar_type(type) || (type && type->tag == ir::TypeTag::POINTER);
}

inline ir::Type default_variadic_abi_type(ir::Type type) {
    if (!type) {
        return nullptr;
    }
    switch (type->tag) {
        case ir::TypeTag::INT8:
        case ir::TypeTag::INT16: {
            auto widened = std::make_shared<ir::TypeKind>();
            widened->tag = ir::TypeTag::INT32;
            return widened;
        }
        default:
            return type;
    }
}

inline BackendError classify_call_arguments(const std::vector<ir::Type>& arg_types,
                                            bool is_variadic,
                                            std::size_t fixed_arg_count,
                                            CallArgLayout& layout) {
    std::size_t next_int_reg = 0;
    std::size_t next_float_reg = 0;
    std::size_t next_stack_offset = 0;

    layout.locations.clear();
    layout.locations.reserve(arg_types.size());

    for (std::size_t i = 0; i < arg_types.size(); ++i) {
        auto type = arg_types[i];
        const bool is_variadic_arg = is_variadic && i >= fixed_arg_count;
        auto abi_type = is_variadic_arg ? default_variadic_abi_type(type) : type;
        RISCV64ArgLocation loc;
        loc.type = abi_type;

        if (is_variadic_arg && is_float_scalar_type(type)) {
            return {BackendErrorCode::UNSUPPORTED_TYPE,
                    "floating-point variadic call arguments are unsupported"};
        }

        if (!is_variadic_arg && is_float_scalar_type(abi_type) && next_float_reg < 8) {
            loc.kind = RISCV64ArgLocation::Kind::FloatReg;
            loc.index = next_float_reg++;
            layout.locations.push_back(loc);
            continue;
        }

        if (!is_integer_abi_scalar_type(abi_type) && !is_float_scalar_type(abi_type)) {
            return {BackendErrorCode::UNSUPPORTED_TYPE,
                    "only integer/pointer and FLOAT/DOUBLE scalar call arguments are supported"};
        }

        if (next_int_reg < 8) {
            loc.kind = RISCV64ArgLocation::Kind::IntReg;
            loc.index = next_int_reg++;
        } else {
            loc.kind = RISCV64ArgLocation::Kind::Stack;
            loc.stack_offset = next_stack_offset;
            next_stack_offset += 8;
        }
        layout.locations.push_back(loc);
    }

    layout.stack_arg_size = align_to(next_stack_offset, 8);
    return BackendError::success();
}

inline BackendError classify_function_arguments(const ir::Function& function,
                                                std::vector<RISCV64ArgLocation>& locations) {
    if (!function || !function->ty || function->ty->tag != ir::TypeTag::FUNCTION) {
        return {BackendErrorCode::INVALID_PROGRAM, "function type is missing or invalid"};
    }

    auto fn_type = std::get<std::shared_ptr<ir::TypeKind::function>>(function->ty->data);
    if (!fn_type) {
        return {BackendErrorCode::INVALID_PROGRAM, "function type payload is missing"};
    }

    std::vector<ir::Type> param_types;
    param_types.reserve(fn_type->params.buffer.size());
    for (const auto& item : fn_type->params.buffer) {
        if (!std::holds_alternative<ir::Type>(item)) {
            continue;
        }
        param_types.push_back(std::get<ir::Type>(item));
    }

    CallArgLayout layout;
    auto result = classify_call_arguments(param_types, false, param_types.size(), layout);
    if (!result.ok()) {
        return result;
    }
    locations = std::move(layout.locations);
    return BackendError::success();
}

inline BackendError classify_call_arguments_for_call(const ir::Function& callee,
                                                     const std::vector<ir::Type>& provided_arg_types,
                                                     CallArgLayout& layout) {
    bool is_variadic = false;
    std::size_t fixed_arg_count = provided_arg_types.size();

    if (callee && callee->ty && callee->ty->tag == ir::TypeTag::FUNCTION) {
        auto fn_type =
            std::get<std::shared_ptr<ir::TypeKind::function>>(callee->ty->data);
        if (fn_type) {
            fixed_arg_count = 0;
            for (const auto& item : fn_type->params.buffer) {
                if (std::holds_alternative<ir::Type>(item)) {
                    ++fixed_arg_count;
                }
            }
            is_variadic = fn_type->is_variadic;
        }
    }

    return classify_call_arguments(provided_arg_types, is_variadic, fixed_arg_count, layout);
}

inline const char* float_move_suffix(ir::Type type) {
    return float_suffix(type);
}

} // namespace rcc::backend::detail