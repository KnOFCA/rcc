// Author: KnOFCA

#include "FrontSymtab.h"
#include <cassert>
#include <stdexcept>

namespace rcc::symtab {

// -------------------------------------------------------------
// TypeDesc helpers
// -------------------------------------------------------------

std::shared_ptr<TypeDesc> TypeDesc::createBuiltin(BuiltinType bt,
                                                   TypeQualifiers quals,
                                                   StorageClass sc) {
    auto t = std::make_shared<TypeDesc>();
    t->category = TypeCategory::Builtin;
    t->builtinType = bt;
    t->qualifiers = quals;
    t->storageClass = sc;
    return t;
}

std::shared_ptr<TypeDesc> TypeDesc::createPointer(
    std::shared_ptr<TypeDesc> base,
    TypeQualifiers quals) {
    assert(base && "base type for pointer must not be null");
    auto t = std::make_shared<TypeDesc>();
    t->category = TypeCategory::Pointer;
    t->baseType = std::move(base);
    t->qualifiers = quals;
    return t;
}

std::shared_ptr<TypeDesc> TypeDesc::createArray(
    std::shared_ptr<TypeDesc> elem, std::size_t size) {
    assert(elem && "element type for array must not be null");
    auto t = std::make_shared<TypeDesc>();
    t->category = TypeCategory::Array;
    t->baseType = std::move(elem);
    t->arraySize = size;
    return t;
}

std::shared_ptr<TypeDesc> TypeDesc::createFunction(
    std::shared_ptr<TypeDesc> ret,
    std::vector<std::shared_ptr<ParamInfo>> params,
    bool varArgs) {
    assert(ret && "return type for function must not be null");
    auto t = std::make_shared<TypeDesc>();
    t->category = TypeCategory::Function;
    t->returnType = std::move(ret);
    t->params = std::move(params);
    t->hasVarArgs = varArgs;
    return t;
}

std::shared_ptr<TypeDesc> TypeDesc::createTag(TypeCategory cat,
                                               const std::string &name) {
    assert(cat == TypeCategory::Struct ||
           cat == TypeCategory::Union ||
           cat == TypeCategory::Enum);
    auto t = std::make_shared<TypeDesc>();
    t->category = cat;
    t->tagName = name;
    return t;
}

bool TypeDesc::isIntegerType() const {
    if (category != TypeCategory::Builtin) return false;
    switch (builtinType) {
        case BuiltinType::Char:
        case BuiltinType::Short:
        case BuiltinType::Int:
        case BuiltinType::Long:
        case BuiltinType::UnsignedChar:
        case BuiltinType::UnsignedShort:
        case BuiltinType::UnsignedInt:
        case BuiltinType::UnsignedLong:
            return true;
        default:
            return false;
    }
}

bool TypeDesc::isFloatType() const {
    if (category != TypeCategory::Builtin) return false;
    return builtinType == BuiltinType::Float ||
           builtinType == BuiltinType::Double;
}

bool TypeDesc::isArithmeticType() const {
    return isIntegerType() || isFloatType();
}

bool TypeDesc::isPointerType() const {
    return category == TypeCategory::Pointer;
}

bool TypeDesc::isArrayType() const {
    return category == TypeCategory::Array;
}

bool TypeDesc::isFunctionType() const {
    return category == TypeCategory::Function;
}

bool TypeDesc::isVoidType() const {
    return category == TypeCategory::Builtin &&
           builtinType == BuiltinType::Void;
}

std::shared_ptr<TypeDesc> TypeDesc::getElementType() const {
    if (category == TypeCategory::Pointer ||
        category == TypeCategory::Array) {
        return baseType;
    }
    return nullptr;
}

static std::size_t builtinSize(BuiltinType bt) {
    switch (bt) {
        case BuiltinType::Char:
        case BuiltinType::UnsignedChar:
            return 1;
        case BuiltinType::Short:
        case BuiltinType::UnsignedShort:
            return 2;
        case BuiltinType::Int:
        case BuiltinType::UnsignedInt:
            return 4;
        case BuiltinType::Long:
        case BuiltinType::UnsignedLong:
            return 8;
        case BuiltinType::Float:
            return 4;
        case BuiltinType::Double:
            return 8;
        case BuiltinType::Void:
            return 1; // treat void as 1 for pointer arithmetic
    }
    return 0;
}

static std::size_t builtinAlign(BuiltinType bt) {
    // simple ABI: alignment equals size for these types
    return builtinSize(bt);
}

std::size_t TypeDesc::getSize() const {
    switch (category) {
        case TypeCategory::Builtin:
            return builtinSize(builtinType);
        case TypeCategory::Pointer:
            return sizeof(void*);
        case TypeCategory::Array:
            if (baseType) {
                return baseType->getSize() * arraySize;
            }
            return 0;
        case TypeCategory::Function:
            return sizeof(void*); // function pointers
        case TypeCategory::Struct:
        case TypeCategory::Union:
        case TypeCategory::Enum:
            // TODO:
            // incomplete; size to be filled during semantic analysis
            return 0;
    }
    return 0;
}

std::size_t TypeDesc::getAlignment() const {
    switch (category) {
        case TypeCategory::Builtin:
            return builtinAlign(builtinType);
        case TypeCategory::Pointer:
            return sizeof(void*);
        case TypeCategory::Array:
            if (baseType) {
                return baseType->getAlignment();
            }
            return 0;
        case TypeCategory::Function:
            return sizeof(void*);
        case TypeCategory::Struct:
        case TypeCategory::Union:
        case TypeCategory::Enum:
            return 0; // to be computed later
    }
    return 0;
}

// -------------------------------------------------------------
// Scope implementation
// -------------------------------------------------------------

Scope::Scope(Scope *parent) : parent_(parent) {}

bool Scope::insert(const std::shared_ptr<Symbol> &sym) {
    if (!sym) return false;
    auto it = objects_.find(sym->name);
    if (it != objects_.end()) return false;
    objects_[sym->name] = sym;
    return true;
}

bool Scope::insertTag(const std::shared_ptr<TagSymbol> &sym) {
    if (!sym) return false;
    auto it = tags_.find(sym->name);
    if (it != tags_.end()) return false;
    tags_[sym->name] = sym;
    return true;
}

std::shared_ptr<Symbol> Scope::lookupLocal(const std::string &name) {
    auto it = objects_.find(name);
    if (it != objects_.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<TagSymbol> Scope::lookupTagLocal(const std::string &name) {
    auto it = tags_.find(name);
    if (it != tags_.end()) {
        return it->second;
    }
    return nullptr;
}

std::shared_ptr<Symbol> Scope::lookup(const std::string &name) {
    auto sym = lookupLocal(name);
    if (sym) return sym;
    if (parent_) return parent_->lookup(name);
    throw std::runtime_error("ERROR: can't find name " + name);
    return nullptr;
}

std::shared_ptr<TagSymbol> Scope::lookupTag(const std::string &name) {
    auto sym = lookupTagLocal(name);
    if (sym) return sym;
    if (parent_) return parent_->lookupTag(name);
    throw std::runtime_error("ERROR: can't find tag " + name);
    return nullptr;
}

// -------------------------------------------------------------
// SymbolTable implementation
// -------------------------------------------------------------

SymbolTable::SymbolTable() {
    // create global (root) scope
    enterScope();
}

void SymbolTable::enterScope() {
    scopes_.push_back(
        std::make_unique<Scope>(current_));
    current_ = scopes_.back().get();
}

void SymbolTable::leaveScope() {
    if (!scopes_.empty()) {
        scopes_.pop_back();
        current_ = scopes_.empty() ? nullptr : scopes_.back().get();
    }
}

Scope *SymbolTable::currentScope() { return current_; }

bool SymbolTable::define(const std::shared_ptr<Symbol> &sym) {
    if (!current_) return false;
    return current_->insert(sym);
}

bool SymbolTable::defineTag(const std::shared_ptr<TagSymbol> &sym) {
    if (!current_) return false;
    return current_->insertTag(sym);
}

std::shared_ptr<Symbol> SymbolTable::lookup(const std::string &name) {
    if (!current_) return nullptr;
    return current_->lookup(name);
}

std::shared_ptr<TagSymbol> SymbolTable::lookupTag(const std::string &name) {
    if (!current_) return nullptr;
    return current_->lookupTag(name);
}

} // namespace rcc::symtab