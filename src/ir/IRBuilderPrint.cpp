#include "IRBuilder.h"
#include "IRVisitor.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <variant>

namespace rcc::ir {

namespace {

class IRTextDumper : public IRVisitor<IRTextDumper> {
  public:
    explicit IRTextDumper(std::ostream& out) : out_(out) {}

    void dump_program(const std::shared_ptr<Program>& program) {
        visit_program(program);
    }

    bool TraverseProgram(const std::shared_ptr<Program>& program) {
        if (!program) return true;

        bool hasGlobal = false;
        if (!detail::visit_slice_items(program->values, SliceItemKind::VALUE,
                                       [&](const Slice::ItemsPtr& item) {
                                           Value value = std::get<Value>(item);
                                           if (!value || value->kind.tag != ValueTag::GLOBAL_ALLOC) {
                                               return true;
                                           }
                                           hasGlobal = true;
                                           return visit_value(value);
                                       })) {
            return false;
        }

        bool firstFunc = true;
        return detail::visit_slice_items(program->funcs, SliceItemKind::FUNCTION,
                                         [&](const Slice::ItemsPtr& item) {
                                             Function func = std::get<Function>(item);
                                             if (!func) return true;
                                             if (hasGlobal || !firstFunc) {
                                                 out_ << '\n';
                                             }
                                             firstFunc = false;
                                             return visit_function(func);
                                         });
    }

    bool TraverseFunction(Function func) {
        if (!func) return true;

        if (func->bbs.buffer.empty()) {
            out_ << "decl " << function_name(func) << "(" << function_param_types(func)
                 << "): " << function_return_type(func) << "\n";
            return true;
        }

        out_ << "fun " << function_name(func) << "(" << function_param_types(func)
             << "): " << function_return_type(func) << " {\n";

        bool ok = detail::visit_slice_items(func->bbs, SliceItemKind::BASIC_BLOCK,
                                            [&](const Slice::ItemsPtr& item) {
                                                return visit_basic_block(std::get<BasicBlock>(item));
                                            });
        if (!ok) return false;

        out_ << "}";
        return true;
    }

    bool TraverseBasicBlock(BasicBlock bb) {
        if (!bb) return true;

        out_ << block_name(bb) << ":\n";
        return detail::visit_slice_items(bb->insts, SliceItemKind::VALUE,
                                         [&](const Slice::ItemsPtr& item) {
                                             return visit_value(std::get<Value>(item));
                                         });
    }

    bool TraverseValue(Value value) {
        (void)value;
        return true;
    }

    bool VisitGlobalAlloc(Value global) {
        if (!global || global->kind.tag != ValueTag::GLOBAL_ALLOC) return true;

        const auto& galloc = std::get<GlobalAlloc>(global->kind.data);
        out_ << "global " << value_name(global) << " = alloc "
             << type_to_string(pointee_type(global->ty)) << ", "
             << inline_value(galloc.init) << "\n";
        return true;
    }

    bool VisitAlloc(Value inst) {
        begin_inst(inst);
        out_ << "alloc " << type_to_string(pointee_type(inst ? inst->ty : nullptr)) << "\n";
        return true;
    }

    bool VisitLoad(Value inst) {
        begin_inst(inst);
        const auto& load = std::get<Load>(inst->kind.data);
        out_ << "load " << inline_value(load.src) << "\n";
        return true;
    }

    bool VisitStore(Value inst) {
        begin_inst(inst);
        const auto& store = std::get<Store>(inst->kind.data);
        out_ << "store " << inline_value(store.value)
             << ", " << inline_value(store.dest) << "\n";
        return true;
    }

    bool VisitGetPtr(Value inst) {
        begin_inst(inst);
        const auto& gp = std::get<GetPtr>(inst->kind.data);
        out_ << "getptr " << inline_value(gp.src)
             << ", " << inline_value(gp.index) << "\n";
        return true;
    }

    bool VisitGetElemPtr(Value inst) {
        begin_inst(inst);
        const auto& gep = std::get<GetElemPtr>(inst->kind.data);
        out_ << "getelemptr " << inline_value(gep.src)
             << ", " << inline_value(gep.index) << "\n";
        return true;
    }

    bool VisitBinary(Value inst) {
        begin_inst(inst);
        const auto& binary = std::get<Binary>(inst->kind.data);
        out_ << binary_op_to_string(binary.op) << " "
             << inline_value(binary.lhs) << ", "
             << inline_value(binary.rhs) << "\n";
        return true;
    }

    bool VisitCast(Value inst) {
        begin_inst(inst);
        const auto& cast = std::get<Cast>(inst->kind.data);
        out_ << cast_op_to_string(cast.op) << " "
             << inline_value(cast.value) << "\n";
        return true;
    }

    bool VisitBranch(Value inst) {
        begin_inst(inst);
        const auto& br = std::get<Branch>(inst->kind.data);
        out_ << "br " << inline_value(br.cond)
             << ", " << block_name(br.true_bb)
             << ", " << block_name(br.false_bb) << "\n";
        return true;
    }

    bool VisitJump(Value inst) {
        begin_inst(inst);
        const auto& jump = std::get<Jump>(inst->kind.data);
        out_ << "jump " << block_name(jump.target) << "\n";
        return true;
    }

    bool VisitCall(Value inst) {
        begin_inst(inst);
        const auto& call = std::get<Call>(inst->kind.data);
        out_ << "call " << function_name(call.callee) << "(";

        bool first = true;
        for (const auto& item : call.args.buffer) {
            if (!std::holds_alternative<Value>(item)) continue;
            if (!first) out_ << ", ";
            first = false;
            out_ << inline_value(std::get<Value>(item));
        }
        out_ << ")\n";
        return true;
    }

    bool VisitReturn(Value inst) {
        begin_inst(inst);
        const auto& ret = std::get<FuncReturn>(inst->kind.data);
        if (ret.value) {
            out_ << "ret " << inline_value(ret.value) << "\n";
        } else {
            out_ << "ret\n";
        }
        return true;
    }

    bool VisitNumber(Value inst) {
        begin_inst(inst);
        out_ << inline_value(inst) << "\n";
        return true;
    }

    bool VisitZeroInit(Value inst) {
        begin_inst(inst);
        (void)inst;
        out_ << "zeroinit\n";
        return true;
    }

    bool VisitUndef(Value inst) {
        begin_inst(inst);
        (void)inst;
        out_ << "undef\n";
        return true;
    }

    bool VisitAggregate(Value inst) {
        begin_inst(inst);
        out_ << aggregate_to_string(std::get<Aggregate>(inst->kind.data)) << "\n";
        return true;
    }

    bool VisitFuncArgRef(Value inst) {
        begin_inst(inst);
        const auto& ref = std::get<FuncArgRef>(inst->kind.data);
        out_ << "%arg" << ref.index << "\n";
        return true;
    }

    bool VisitBlockArgRef(Value inst) {
        begin_inst(inst);
        const auto& ref = std::get<BlockArgRef>(inst->kind.data);
        out_ << "%barg" << ref.index << "\n";
        return true;
    }

  private:
    std::ostream& out_;
    std::unordered_map<const ValueData*, std::string> valueNames_;
    std::unordered_map<const BasicBlockData*, std::string> bbNames_;
    std::size_t valueCounter_ = 0;
    std::size_t bbCounter_ = 0;

    void begin_inst(Value inst) {
        out_ << "  ";
        if (has_result(inst)) {
            out_ << value_name(inst) << " = ";
        }
    }

    std::string function_name(Function func) const {
        if (func && func->name && !func->name->empty()) {
            return *func->name;
        }
        return "@anon_fn";
    }

    std::string function_return_type(Function func) {
        if (!func || !func->ty || func->ty->tag != TypeTag::FUNCTION) {
            return "unit";
        }
        auto fnType = std::get<std::shared_ptr<TypeKind::function>>(func->ty->data);
        if (!fnType) return "unit";
        return type_to_string(fnType->ret);
    }

    std::string function_param_types(Function func) {
        if (!func || !func->ty || func->ty->tag != TypeTag::FUNCTION) {
            return "";
        }
        auto fnType = std::get<std::shared_ptr<TypeKind::function>>(func->ty->data);
        if (!fnType) return "";

        std::ostringstream oss;
        bool first = true;
        for (const auto& param : fnType->params.buffer) {
            if (!std::holds_alternative<Type>(param)) continue;
            if (!first) oss << ", ";
            first = false;
            oss << type_to_string(std::get<Type>(param));
        }
        if (fnType->is_variadic) {
            if (!first) oss << ", ";
            oss << "...";
        }
        return oss.str();
    }

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
                if (fn->is_variadic) {
                    if (!first) oss << ", ";
                    oss << "...";
                }
                oss << ") -> " << type_to_string(fn->ret);
                return oss.str();
            }
        }
        return "unit";
    }

    std::string number_to_string(const Number& num) const {
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
            case ValueTag::FUNC_ARG_REF: {
                const auto& ref = std::get<FuncArgRef>(value->kind.data);
                return "%arg" + std::to_string(ref.index);
            }
            case ValueTag::BLOCK_ARG_REF: {
                const auto& ref = std::get<BlockArgRef>(value->kind.data);
                return "%barg" + std::to_string(ref.index);
            }
            default:
                return value_name(value);
        }
    }

    std::string binary_op_to_string(BinaryOp op) const {
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

    std::string cast_op_to_string(CastOp op) const {
        switch (op) {
            case CastOp::SITOFP: return "sitofp";
            case CastOp::FPTOSI: return "fptosi";
            case CastOp::INTCAST: return "intcast";
            case CastOp::FPEXT: return "fpext";
            case CastOp::FPTRUNC: return "fptrunc";
        }
        return "sitofp";
    }

    Type pointee_type(Type ptrTy) const {
        if (!ptrTy || ptrTy->tag != TypeTag::POINTER) return nullptr;
        auto ptr = std::get<std::shared_ptr<TypeKind::pointer>>(ptrTy->data);
        if (!ptr) return nullptr;
        return ptr->base;
    }

    bool has_result(Value inst) const {
        if (!inst) return false;
        switch (inst->kind.tag) {
            case ValueTag::ALLOC:
            case ValueTag::LOAD:
            case ValueTag::GET_PTR:
            case ValueTag::GET_ELEM_PTR:
            case ValueTag::BINARY:
            case ValueTag::CAST:
            case ValueTag::CALL:
                return true;
            default:
                return false;
        }
    }
};

} // namespace

ErrorCode IRBuilder::dump_to_stdout() {
    if (!program) {
        return ErrorCode::INVALID_PROGRAM;
    }

    IRTextDumper dumper(std::cout);
    dumper.dump_program(program);

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
    dumper.dump_program(program);

    ofs.flush();
    if (!ofs.good()) {
        return ErrorCode::IO_ERROR;
    }
    return ErrorCode::SUCCESS;
}

} // namespace rcc::ir
