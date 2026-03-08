#include "IRBuilder.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <variant>

namespace rcc::ir {

namespace {

class IRTextDumper {
  public:
    explicit IRTextDumper(std::ostream& out) : out_(out) {}

    void dump_program(const Program& program) {
        for (const auto& item : program.values.buffer) {
            if (std::holds_alternative<Value>(item)) {
                dump_global(std::get<Value>(item));
            }
        }

        if (!program.values.buffer.empty() && !program.funcs.buffer.empty()) {
            out_ << '\n';
        }

        bool firstFunc = true;
        for (const auto& item : program.funcs.buffer) {
            if (!std::holds_alternative<Function>(item)) {
                continue;
            }
            if (!firstFunc) {
                out_ << '\n';
            }
            firstFunc = false;
            dump_function(std::get<Function>(item));
        }
    }

  private:
    std::ostream& out_;
    std::unordered_map<const ValueData*, std::string> valueNames_;
    std::unordered_map<const BasicBlockData*, std::string> bbNames_;
    std::size_t valueCounter_ = 0;
    std::size_t bbCounter_ = 0;

    std::string type_to_string(Type ty) {
        if (!ty) return "unit";
        switch (ty->tag) {
            case TypeTag::INT8: return "i8";
            case TypeTag::INT16: return "i16";
            case TypeTag::INT32: return "i32";
            case TypeTag::INT64: return "i64";
            case TypeTag::FLOAT: return "f32";
            case TypeTag::DOUBLE: return "f64";
            case TypeTag::UNIT: return "unit";
            case TypeTag::ARRAY: {
                auto arr = std::get<std::shared_ptr<TypeKind::array>>(ty->data);
                if (!arr) return "[unit, 0]";
                std::ostringstream oss;
                oss << "[" << type_to_string(arr->base) << ", " << arr->len << "]";
                return oss.str();
            }
            case TypeTag::POINTER: {
                auto ptr = std::get<std::shared_ptr<TypeKind::pointer>>(ty->data);
                if (!ptr) return "*unit";
                return "*" + type_to_string(ptr->base);
            }
            case TypeTag::FUNCTION: {
                auto fn = std::get<std::shared_ptr<TypeKind::function>>(ty->data);
                if (!fn) return "() -> unit";
                std::ostringstream oss;
                oss << "(";
                bool first = true;
                for (const auto& param : fn->params.buffer) {
                    if (!std::holds_alternative<Type>(param)) continue;
                    if (!first) oss << ", ";
                    first = false;
                    oss << type_to_string(std::get<Type>(param));
                }
                oss << ") -> " << type_to_string(fn->ret);
                return oss.str();
            }
        }
        return "unit";
    }

    std::string number_to_string(const Number& num) {
        std::ostringstream oss;
        if (std::holds_alternative<Integer8>(num.num)) {
            oss << static_cast<int>(std::get<Integer8>(num.num).value);
        } else if (std::holds_alternative<Integer16>(num.num)) {
            oss << std::get<Integer16>(num.num).value;
        } else if (std::holds_alternative<Integer32>(num.num)) {
            oss << std::get<Integer32>(num.num).value;
        } else if (std::holds_alternative<Integer64>(num.num)) {
            oss << std::get<Integer64>(num.num).value;
        } else if (std::holds_alternative<Float>(num.num)) {
            oss << std::get<Float>(num.num).value;
        } else if (std::holds_alternative<Double>(num.num)) {
            oss << std::get<Double>(num.num).value;
        }
        return oss.str();
    }

    std::string block_name(BasicBlock bb) {
        if (!bb) return "%bb_null";
        if (bb->name && !bb->name->empty()) return *bb->name;

        auto it = bbNames_.find(bb.get());
        if (it != bbNames_.end()) return it->second;

        std::string generated = "%bb" + std::to_string(bbCounter_++);
        bbNames_[bb.get()] = generated;
        return generated;
    }

    std::string value_name(Value value) {
        if (!value) return "undef";
        if (value->name && !value->name->empty()) return *value->name;

        auto it = valueNames_.find(value.get());
        if (it != valueNames_.end()) return it->second;

        std::string generated = "%tmp" + std::to_string(valueCounter_++);
        valueNames_[value.get()] = generated;
        return generated;
    }

    std::string inline_value(Value value) {
        if (!value) return "undef";
        switch (value->kind.tag) {
            case ValueTag::NUMBER:
                return number_to_string(std::get<Number>(value->kind.data));
            case ValueTag::ZERO_INIT:
                return "zeroinit";
            case ValueTag::UNDEF:
                return "undef";
            case ValueTag::AGGREGATE:
                return aggregate_to_string(std::get<Aggregate>(value->kind.data));
            default:
                return value_name(value);
        }
    }

    std::string aggregate_to_string(const Aggregate& agg) {
        std::ostringstream oss;
        oss << "{";
        bool first = true;
        for (const auto& elem : agg.elems.buffer) {
            if (!std::holds_alternative<Value>(elem)) continue;
            if (!first) oss << ", ";
            first = false;
            oss << inline_value(std::get<Value>(elem));
        }
        oss << "}";
        return oss.str();
    }

    std::string binary_op_to_string(BinaryOp op) {
        switch (op) {
            case BinaryOp::NOT_EQ: return "ne";
            case BinaryOp::EQ: return "eq";
            case BinaryOp::GT: return "gt";
            case BinaryOp::LT: return "lt";
            case BinaryOp::GE: return "ge";
            case BinaryOp::LE: return "le";
            case BinaryOp::ADD: return "add";
            case BinaryOp::SUB: return "sub";
            case BinaryOp::MUL: return "mul";
            case BinaryOp::DIV: return "div";
            case BinaryOp::MOD: return "mod";
            case BinaryOp::AND: return "and";
            case BinaryOp::OR: return "or";
            case BinaryOp::XOR: return "xor";
            case BinaryOp::SHL: return "shl";
            case BinaryOp::SHR: return "shr";
            case BinaryOp::SAR: return "sar";
        }
        return "add";
    }

    std::string cast_op_to_string(CastOp op) {
        switch (op) {
            case CastOp::SITOFP: return "sitofp";
            case CastOp::FPTOSI: return "fptosi";
        }
        return "sitofp";
    }

    Type pointee_type(Type ptrTy) {
        if (!ptrTy || ptrTy->tag != TypeTag::POINTER) return nullptr;
        auto ptr = std::get<std::shared_ptr<TypeKind::pointer>>(ptrTy->data);
        if (!ptr) return nullptr;
        return ptr->base;
    }

    bool has_result(Value inst) {
        if (!inst) return false;
        switch (inst->kind.tag) {
            case ValueTag::ALLOC:
            case ValueTag::LOAD:
            case ValueTag::GET_PTR:
            case ValueTag::GET_ELEM_PTR:
            case ValueTag::BINARY:
            case ValueTag::CAST:
                return true;
            case ValueTag::CALL:
                return true;
            default:
                return false;
        }
    }

    void dump_global(Value global) {
        if (!global || global->kind.tag != ValueTag::GLOBAL_ALLOC) {
            return;
        }

        const auto& galloc = std::get<GlobalAlloc>(global->kind.data);
        std::string name = (global->name && !global->name->empty())
                               ? *global->name
                               : value_name(global);
        Type baseTy = pointee_type(global->ty);

        out_ << "global " << name << " = alloc "
             << type_to_string(baseTy) << ", "
             << inline_value(galloc.init) << "\n";
    }

    void dump_function(Function func) {
        if (!func) return;

        Type retTy = nullptr;
        if (func->ty && func->ty->tag == TypeTag::FUNCTION) {
            auto fnType = std::get<std::shared_ptr<TypeKind::function>>(func->ty->data);
            if (fnType) retTy = fnType->ret;
        }

        std::string fname = (func->name && !func->name->empty())
                                ? *func->name
                                : "@anon_fn";

        out_ << "fun " << fname << "(): " << type_to_string(retTy) << " {\n";

        for (const auto& item : func->bbs.buffer) {
            if (!std::holds_alternative<BasicBlock>(item)) {
                continue;
            }
            dump_block(std::get<BasicBlock>(item));
        }

        out_ << "}";
    }

    void dump_block(BasicBlock bb) {
        if (!bb) return;

        out_ << block_name(bb) << ":\n";
        for (const auto& instItem : bb->insts.buffer) {
            if (!std::holds_alternative<Value>(instItem)) {
                continue;
            }
            dump_inst(std::get<Value>(instItem));
        }
    }

    void dump_inst(Value inst) {
        if (!inst) return;

        out_ << "  ";
        if (has_result(inst)) {
            out_ << value_name(inst) << " = ";
        }

        switch (inst->kind.tag) {
            case ValueTag::ALLOC: {
                out_ << "alloc " << type_to_string(pointee_type(inst->ty));
                break;
            }
            case ValueTag::LOAD: {
                const auto& load = std::get<Load>(inst->kind.data);
                out_ << "load " << inline_value(load.src);
                break;
            }
            case ValueTag::STORE: {
                const auto& store = std::get<Store>(inst->kind.data);
                out_ << "store " << inline_value(store.value)
                     << ", " << inline_value(store.dest);
                break;
            }
            case ValueTag::GET_PTR: {
                const auto& gp = std::get<GetPtr>(inst->kind.data);
                out_ << "getptr " << inline_value(gp.src)
                     << ", " << inline_value(gp.index);
                break;
            }
            case ValueTag::GET_ELEM_PTR: {
                const auto& gep = std::get<GetElemPtr>(inst->kind.data);
                out_ << "getelemptr " << inline_value(gep.src)
                     << ", " << inline_value(gep.index);
                break;
            }
            case ValueTag::BINARY: {
                const auto& binary = std::get<Binary>(inst->kind.data);
                out_ << binary_op_to_string(binary.op) << " "
                     << inline_value(binary.lhs) << ", "
                     << inline_value(binary.rhs);
                break;
            }
            case ValueTag::CAST: {
                const auto& cast = std::get<Cast>(inst->kind.data);
                out_ << cast_op_to_string(cast.op) << " "
                     << inline_value(cast.value);
                break;
            }
            case ValueTag::BRANCH: {
                const auto& br = std::get<Branch>(inst->kind.data);
                out_ << "br " << inline_value(br.cond)
                     << ", " << block_name(br.true_bb)
                     << ", " << block_name(br.false_bb);
                break;
            }
            case ValueTag::JUMP: {
                const auto& jump = std::get<Jump>(inst->kind.data);
                out_ << "jump " << block_name(jump.target);
                break;
            }
            case ValueTag::CALL: {
                const auto& call = std::get<Call>(inst->kind.data);
                std::string callee = (call.callee && call.callee->name && !call.callee->name->empty())
                                         ? *call.callee->name
                                         : "@anon_fn";
                out_ << "call " << callee << "(";
                bool first = true;
                for (const auto& arg : call.args.buffer) {
                    if (!std::holds_alternative<Value>(arg)) continue;
                    if (!first) out_ << ", ";
                    first = false;
                    out_ << inline_value(std::get<Value>(arg));
                }
                out_ << ")";
                break;
            }
            case ValueTag::RETURN: {
                const auto& ret = std::get<FuncReturn>(inst->kind.data);
                if (ret.value) {
                    out_ << "ret " << inline_value(ret.value);
                } else {
                    out_ << "ret";
                }
                break;
            }
            case ValueTag::NUMBER: {
                out_ << inline_value(inst);
                break;
            }
            case ValueTag::ZERO_INIT: {
                out_ << "zeroinit";
                break;
            }
            case ValueTag::UNDEF: {
                out_ << "undef";
                break;
            }
            case ValueTag::AGGREGATE: {
                out_ << aggregate_to_string(std::get<Aggregate>(inst->kind.data));
                break;
            }
            case ValueTag::FUNC_ARG_REF: {
                const auto& ref = std::get<FuncArgRef>(inst->kind.data);
                out_ << "%arg" << ref.index;
                break;
            }
            case ValueTag::BLOCK_ARG_REF: {
                const auto& ref = std::get<BlockArgRef>(inst->kind.data);
                out_ << "%barg" << ref.index;
                break;
            }
            case ValueTag::GLOBAL_ALLOC: {
                out_ << "<global_alloc>";
                break;
            }
        }

        out_ << "\n";
    }
};

} // namespace

ErrorCode IRBuilder::dump_to_stdout() {
    if (!program) {
        return ErrorCode::INVALID_PROGRAM;
    }

    IRTextDumper dumper(std::cout);
    dumper.dump_program(*program);

    if (!std::cout.good()) {
        return ErrorCode::IO_ERROR;
    }
    return ErrorCode::SUCCESS;
}

ErrorCode IRBuilder::dump_to_file(const std::filesystem::path& path) {
    if (!program) {
        return ErrorCode::INVALID_PROGRAM;
    }

    std::ofstream ofs(path);
    if (!ofs.is_open()) {
        return ErrorCode::INVALID_FILE;
    }

    IRTextDumper dumper(ofs);
    dumper.dump_program(*program);

    ofs.flush();
    if (!ofs.good()) {
        return ErrorCode::IO_ERROR;
    }
    return ErrorCode::SUCCESS;
}

} // namespace rcc::ir
