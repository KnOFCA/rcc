// Author: KnOFCA

#pragma once

#include "koopa.h"

#include <functional>
#include <memory>
#include <unordered_set>
#include <variant>

namespace rcc::ir {

namespace detail {

using SliceVisitor = std::function<bool(const Slice::ItemsPtr&)>;

bool visit_slice_items(const Slice& slice, SliceItemKind expectedKind,
                       const SliceVisitor& visitor);

} // namespace detail

template <class Derived>
class RecursiveIRVisitor {
  protected:
    Derived& derived() { return static_cast<Derived&>(*this); }
    const Derived& derived() const { return static_cast<const Derived&>(*this); }

  private:
    std::unordered_set<const Program*> activePrograms_;
    std::unordered_set<const FunctionData*> activeFunctions_;
    std::unordered_set<const BasicBlockData*> activeBasicBlocks_;
    std::unordered_set<const ValueData*> activeValues_;
    std::unordered_set<const TypeKind*> activeTypes_;

  public:
    RecursiveIRVisitor() = default;
    virtual ~RecursiveIRVisitor() = default;

    bool visit_program(const std::shared_ptr<Program>& program) {
        if (!program) return true;
        auto [it, inserted] = activePrograms_.insert(program.get());
        if (!inserted) return true;
        bool ok = derived().WalkUpFromProgram(program);
        activePrograms_.erase(it);
        return ok;
    }

    bool visit_function(Function func) {
        if (!func) return true;
        auto [it, inserted] = activeFunctions_.insert(func.get());
        if (!inserted) return true;
        bool ok = derived().WalkUpFromFunction(func);
        activeFunctions_.erase(it);
        return ok;
    }

    bool visit_basic_block(BasicBlock bb) {
        if (!bb) return true;
        auto [it, inserted] = activeBasicBlocks_.insert(bb.get());
        if (!inserted) return true;
        bool ok = derived().WalkUpFromBasicBlock(bb);
        activeBasicBlocks_.erase(it);
        return ok;
    }

    bool visit_value(Value value) {
        if (!value) return true;
        auto [it, inserted] = activeValues_.insert(value.get());
        if (!inserted) return true;
        bool ok = derived().WalkUpFromValue(value);
        activeValues_.erase(it);
        return ok;
    }

    bool visit_type(Type type) {
        if (!type) return true;
        auto [it, inserted] = activeTypes_.insert(type.get());
        if (!inserted) return true;
        bool ok = derived().WalkUpFromType(type);
        activeTypes_.erase(it);
        return ok;
    }

    bool WalkUpFromProgram(const std::shared_ptr<Program>& program) {
        return derived().VisitProgram(program);
    }

    bool TraverseProgram(const std::shared_ptr<Program>& program) {
        if (!program) return true;
        if (!detail::visit_slice_items(program->values, SliceItemKind::VALUE,
                                       [&](const Slice::ItemsPtr& item) {
                                           return derived().visit_value(std::get<Value>(item));
                                       })) {
            return false;
        }
        return detail::visit_slice_items(program->funcs, SliceItemKind::FUNCTION,
                                         [&](const Slice::ItemsPtr& item) {
                                             return derived().visit_function(std::get<Function>(item));
                                         });
    }

    bool VisitProgram(const std::shared_ptr<Program>& program) {
        if (!program) return true;
        return derived().TraverseProgram(program);
    }

    bool WalkUpFromFunction(Function func) {
        return derived().VisitFunction(func);
    }

    bool TraverseFunction(Function func) {
        if (!func) return true;
        if (!derived().visit_type(func->ty)) return false;
        if (!detail::visit_slice_items(func->params, SliceItemKind::VALUE,
                                       [&](const Slice::ItemsPtr& item) {
                                           return derived().visit_value(std::get<Value>(item));
                                       })) {
            return false;
        }
        return detail::visit_slice_items(func->bbs, SliceItemKind::BASIC_BLOCK,
                                         [&](const Slice::ItemsPtr& item) {
                                             return derived().visit_basic_block(std::get<BasicBlock>(item));
                                         });
    }

    bool VisitFunction(Function func) {
        if (!func) return true;
        return derived().TraverseFunction(func);
    }

    bool WalkUpFromBasicBlock(BasicBlock bb) {
        return derived().VisitBasicBlock(bb);
    }

    bool TraverseBasicBlock(BasicBlock bb) {
        if (!bb) return true;
        if (!detail::visit_slice_items(bb->params, SliceItemKind::VALUE,
                                       [&](const Slice::ItemsPtr& item) {
                                           return derived().visit_value(std::get<Value>(item));
                                       })) {
            return false;
        }
        return detail::visit_slice_items(bb->insts, SliceItemKind::VALUE,
                                         [&](const Slice::ItemsPtr& item) {
                                             return derived().visit_value(std::get<Value>(item));
                                         });
    }

    bool VisitBasicBlock(BasicBlock bb) {
        if (!bb) return true;
        return derived().TraverseBasicBlock(bb);
    }

    bool WalkUpFromType(Type type) {
        if (!derived().VisitType(type)) return false;

        switch (type->tag) {
            case TypeTag::ARRAY:
                return derived().WalkUpFromArrayType(type);
            case TypeTag::POINTER:
                return derived().WalkUpFromPointerType(type);
            case TypeTag::FUNCTION:
                return derived().WalkUpFromFunctionType(type);
            case TypeTag::INT8:
            case TypeTag::INT16:
            case TypeTag::INT32:
            case TypeTag::INT64:
            case TypeTag::FLOAT:
            case TypeTag::DOUBLE:
            case TypeTag::UNIT:
                return derived().WalkUpFromScalarType(type);
        }
        return true;
    }

    bool TraverseType(Type type) {
        (void)type;
        return true;
    }

    bool VisitType(Type type) {
        if (!type) return true;
        return derived().TraverseType(type);
    }

    bool WalkUpFromScalarType(Type type) {
        return derived().VisitScalarType(type);
    }

    bool TraverseScalarType(Type type) {
        (void)type;
        return true;
    }

    bool VisitScalarType(Type type) {
        if (!type) return true;
        return derived().TraverseScalarType(type);
    }

    bool WalkUpFromArrayType(Type type) {
        return derived().VisitArrayType(type);
    }

    bool TraverseArrayType(Type type) {
        if (!type || type->tag != TypeTag::ARRAY) return true;
        const auto& array = std::get<std::shared_ptr<TypeKind::array>>(type->data);
        if (!array) return true;
        return derived().visit_type(array->base);
    }

    bool VisitArrayType(Type type) {
        if (!type) return true;
        return derived().TraverseArrayType(type);
    }

    bool WalkUpFromPointerType(Type type) {
        return derived().VisitPointerType(type);
    }

    bool TraversePointerType(Type type) {
        if (!type || type->tag != TypeTag::POINTER) return true;
        const auto& ptr = std::get<std::shared_ptr<TypeKind::pointer>>(type->data);
        if (!ptr) return true;
        return derived().visit_type(ptr->base);
    }

    bool VisitPointerType(Type type) {
        if (!type) return true;
        return derived().TraversePointerType(type);
    }

    bool WalkUpFromFunctionType(Type type) {
        return derived().VisitFunctionType(type);
    }

    bool TraverseFunctionType(Type type) {
        if (!type || type->tag != TypeTag::FUNCTION) return true;
        const auto& fn = std::get<std::shared_ptr<TypeKind::function>>(type->data);
        if (!fn) return true;
        if (!detail::visit_slice_items(fn->params, SliceItemKind::TYPE,
                                       [&](const Slice::ItemsPtr& item) {
                                           return derived().visit_type(std::get<Type>(item));
                                       })) {
            return false;
        }
        return derived().visit_type(fn->ret);
    }

    bool VisitFunctionType(Type type) {
        if (!type) return true;
        return derived().TraverseFunctionType(type);
    }

    bool WalkUpFromValue(Value value) {
        if (!derived().VisitValue(value)) return false;

        switch (value->kind.tag) {
            case ValueTag::NUMBER:
                return derived().WalkUpFromNumber(value);
            case ValueTag::ZERO_INIT:
                return derived().WalkUpFromZeroInit(value);
            case ValueTag::UNDEF:
                return derived().WalkUpFromUndef(value);
            case ValueTag::AGGREGATE:
                return derived().WalkUpFromAggregate(value);
            case ValueTag::FUNC_ARG_REF:
                return derived().WalkUpFromFuncArgRef(value);
            case ValueTag::BLOCK_ARG_REF:
                return derived().WalkUpFromBlockArgRef(value);
            case ValueTag::ALLOC:
                return derived().WalkUpFromAlloc(value);
            case ValueTag::GLOBAL_ALLOC:
                return derived().WalkUpFromGlobalAlloc(value);
            case ValueTag::LOAD:
                return derived().WalkUpFromLoad(value);
            case ValueTag::STORE:
                return derived().WalkUpFromStore(value);
            case ValueTag::GET_PTR:
                return derived().WalkUpFromGetPtr(value);
            case ValueTag::GET_ELEM_PTR:
                return derived().WalkUpFromGetElemPtr(value);
            case ValueTag::BINARY:
                return derived().WalkUpFromBinary(value);
            case ValueTag::CAST:
                return derived().WalkUpFromCast(value);
            case ValueTag::BRANCH:
                return derived().WalkUpFromBranch(value);
            case ValueTag::JUMP:
                return derived().WalkUpFromJump(value);
            case ValueTag::CALL:
                return derived().WalkUpFromCall(value);
            case ValueTag::RETURN:
                return derived().WalkUpFromReturn(value);
        }
        return true;
    }

    bool TraverseValue(Value value) {
        (void)value;
        return true;
    }

    bool VisitValue(Value value) {
        if (!value) return true;
        return derived().TraverseValue(value);
    }

    bool WalkUpFromNumber(Value value) { return derived().VisitNumber(value); }
    bool TraverseNumber(Value value) { (void)value; return true; }
    bool VisitNumber(Value value) {
        if (!value) return true;
        return derived().TraverseNumber(value);
    }

    bool WalkUpFromZeroInit(Value value) { return derived().VisitZeroInit(value); }
    bool TraverseZeroInit(Value value) { (void)value; return true; }
    bool VisitZeroInit(Value value) {
        if (!value) return true;
        return derived().TraverseZeroInit(value);
    }

    bool WalkUpFromUndef(Value value) { return derived().VisitUndef(value); }
    bool TraverseUndef(Value value) { (void)value; return true; }
    bool VisitUndef(Value value) {
        if (!value) return true;
        return derived().TraverseUndef(value);
    }

    bool WalkUpFromAggregate(Value value) { return derived().VisitAggregate(value); }
    bool TraverseAggregate(Value value) {
        if (!value || value->kind.tag != ValueTag::AGGREGATE) return true;
        const auto& aggregate = std::get<Aggregate>(value->kind.data);
        return detail::visit_slice_items(aggregate.elems, SliceItemKind::VALUE,
                                         [&](const Slice::ItemsPtr& item) {
                                             return derived().visit_value(std::get<Value>(item));
                                         });
    }
    bool VisitAggregate(Value value) {
        if (!value) return true;
        return derived().TraverseAggregate(value);
    }

    bool WalkUpFromFuncArgRef(Value value) { return derived().VisitFuncArgRef(value); }
    bool TraverseFuncArgRef(Value value) { (void)value; return true; }
    bool VisitFuncArgRef(Value value) {
        if (!value) return true;
        return derived().TraverseFuncArgRef(value);
    }

    bool WalkUpFromBlockArgRef(Value value) { return derived().VisitBlockArgRef(value); }
    bool TraverseBlockArgRef(Value value) { (void)value; return true; }
    bool VisitBlockArgRef(Value value) {
        if (!value) return true;
        return derived().TraverseBlockArgRef(value);
    }

    bool WalkUpFromAlloc(Value value) { return derived().VisitAlloc(value); }
    bool TraverseAlloc(Value value) {
        if (!value) return true;
        return derived().visit_type(value->ty);
    }
    bool VisitAlloc(Value value) {
        if (!value) return true;
        return derived().TraverseAlloc(value);
    }

    bool WalkUpFromGlobalAlloc(Value value) { return derived().VisitGlobalAlloc(value); }
    bool TraverseGlobalAlloc(Value value) {
        if (!value || value->kind.tag != ValueTag::GLOBAL_ALLOC) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& global = std::get<GlobalAlloc>(value->kind.data);
        return derived().visit_value(global.init);
    }
    bool VisitGlobalAlloc(Value value) {
        if (!value) return true;
        return derived().TraverseGlobalAlloc(value);
    }

    bool WalkUpFromLoad(Value value) { return derived().VisitLoad(value); }
    bool TraverseLoad(Value value) {
        if (!value || value->kind.tag != ValueTag::LOAD) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& load = std::get<Load>(value->kind.data);
        return derived().visit_value(load.src);
    }
    bool VisitLoad(Value value) {
        if (!value) return true;
        return derived().TraverseLoad(value);
    }

    bool WalkUpFromStore(Value value) { return derived().VisitStore(value); }
    bool TraverseStore(Value value) {
        if (!value || value->kind.tag != ValueTag::STORE) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& store = std::get<Store>(value->kind.data);
        if (!derived().visit_value(store.value)) return false;
        return derived().visit_value(store.dest);
    }
    bool VisitStore(Value value) {
        if (!value) return true;
        return derived().TraverseStore(value);
    }

    bool WalkUpFromGetPtr(Value value) { return derived().VisitGetPtr(value); }
    bool TraverseGetPtr(Value value) {
        if (!value || value->kind.tag != ValueTag::GET_PTR) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& getPtr = std::get<GetPtr>(value->kind.data);
        if (!derived().visit_value(getPtr.src)) return false;
        return derived().visit_value(getPtr.index);
    }
    bool VisitGetPtr(Value value) {
        if (!value) return true;
        return derived().TraverseGetPtr(value);
    }

    bool WalkUpFromGetElemPtr(Value value) { return derived().VisitGetElemPtr(value); }
    bool TraverseGetElemPtr(Value value) {
        if (!value || value->kind.tag != ValueTag::GET_ELEM_PTR) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& getElemPtr = std::get<GetElemPtr>(value->kind.data);
        if (!derived().visit_value(getElemPtr.src)) return false;
        return derived().visit_value(getElemPtr.index);
    }
    bool VisitGetElemPtr(Value value) {
        if (!value) return true;
        return derived().TraverseGetElemPtr(value);
    }

    bool WalkUpFromBinary(Value value) { return derived().VisitBinary(value); }
    bool TraverseBinary(Value value) {
        if (!value || value->kind.tag != ValueTag::BINARY) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& binary = std::get<Binary>(value->kind.data);
        if (!derived().visit_value(binary.lhs)) return false;
        return derived().visit_value(binary.rhs);
    }
    bool VisitBinary(Value value) {
        if (!value) return true;
        return derived().TraverseBinary(value);
    }

    bool WalkUpFromCast(Value value) { return derived().VisitCast(value); }
    bool TraverseCast(Value value) {
        if (!value || value->kind.tag != ValueTag::CAST) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& cast = std::get<Cast>(value->kind.data);
        return derived().visit_value(cast.value);
    }
    bool VisitCast(Value value) {
        if (!value) return true;
        return derived().TraverseCast(value);
    }

    bool WalkUpFromBranch(Value value) { return derived().VisitBranch(value); }
    bool TraverseBranch(Value value) {
        if (!value || value->kind.tag != ValueTag::BRANCH) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& branch = std::get<Branch>(value->kind.data);
        if (!derived().visit_value(branch.cond)) return false;
        if (!derived().visit_basic_block(branch.true_bb)) return false;
        if (!detail::visit_slice_items(branch.true_args, SliceItemKind::VALUE,
                                       [&](const Slice::ItemsPtr& item) {
                                           return derived().visit_value(std::get<Value>(item));
                                       })) {
            return false;
        }
        if (!derived().visit_basic_block(branch.false_bb)) return false;
        return detail::visit_slice_items(branch.false_args, SliceItemKind::VALUE,
                                         [&](const Slice::ItemsPtr& item) {
                                             return derived().visit_value(std::get<Value>(item));
                                         });
    }
    bool VisitBranch(Value value) {
        if (!value) return true;
        return derived().TraverseBranch(value);
    }

    bool WalkUpFromJump(Value value) { return derived().VisitJump(value); }
    bool TraverseJump(Value value) {
        if (!value || value->kind.tag != ValueTag::JUMP) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& jump = std::get<Jump>(value->kind.data);
        if (!derived().visit_basic_block(jump.target)) return false;
        return detail::visit_slice_items(jump.args, SliceItemKind::VALUE,
                                         [&](const Slice::ItemsPtr& item) {
                                             return derived().visit_value(std::get<Value>(item));
                                         });
    }
    bool VisitJump(Value value) {
        if (!value) return true;
        return derived().TraverseJump(value);
    }

    bool WalkUpFromCall(Value value) { return derived().VisitCall(value); }
    bool TraverseCall(Value value) {
        if (!value || value->kind.tag != ValueTag::CALL) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& call = std::get<Call>(value->kind.data);
        if (!derived().visit_function(call.callee)) return false;
        return detail::visit_slice_items(call.args, SliceItemKind::VALUE,
                                         [&](const Slice::ItemsPtr& item) {
                                             return derived().visit_value(std::get<Value>(item));
                                         });
    }
    bool VisitCall(Value value) {
        if (!value) return true;
        return derived().TraverseCall(value);
    }

    bool WalkUpFromReturn(Value value) { return derived().VisitReturn(value); }
    bool TraverseReturn(Value value) {
        if (!value || value->kind.tag != ValueTag::RETURN) return true;
        if (!derived().visit_type(value->ty)) return false;
        const auto& ret = std::get<FuncReturn>(value->kind.data);
        return derived().visit_value(ret.value);
    }
    bool VisitReturn(Value value) {
        if (!value) return true;
        return derived().TraverseReturn(value);
    }
};

template <class Derived>
using IRVisitor = RecursiveIRVisitor<Derived>;

} // namespace rcc::ir