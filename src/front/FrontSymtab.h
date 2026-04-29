// Author: KnOFCA

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include "AST.h"
#include "ir/koopa.h"  // for rcc::ir::Value

namespace rcc::symtab {

/* ============================================================
 * Symbol kind
 * ============================================================ */
enum class SymbolKind {
    Object,
    Function,
    Typedef,
    EnumConst
};

enum class TagKind {
    Struct,
    Union,
    Enum
};

/* ============================================================
 * TypeDesc - 类型描述符
 *
 * 支持基础类型、指针、数组、函数类型的完整描述。
 * 可直接用于生成 Koopa IR 的 Type。
 * ============================================================ */

enum class TypeCategory {
    Builtin,
    Pointer,
    Array,
    Function,
    Struct,
    Union,
    Enum
};

enum class BuiltinType {
    Void,
    Char,
    Short,
    Int,
    Long,
    Float,
    Double,
    UnsignedChar,
    UnsignedShort,
    UnsignedInt,
    UnsignedLong
};

struct TypeQualifiers {
    bool isConst = false;
    bool isVolatile = false;
    bool isRestrict = false;

    TypeQualifiers() = default;
    TypeQualifiers(bool c, bool v = false, bool r = false)
        : isConst(c), isVolatile(v), isRestrict(r) {}
};

enum class StorageClass {
    None,
    Auto,
    Register,
    Static,
    Extern,
    Typedef
};

struct ParamInfo {
    std::shared_ptr<struct TypeDesc> type;
    std::string name;

    ParamInfo(std::shared_ptr<struct TypeDesc> t, const std::string& n = "")
        : type(t), name(n) {}
};

struct TypeDesc {
    TypeCategory category = TypeCategory::Builtin;
    
    // 基础类型
    BuiltinType builtinType = BuiltinType::Int;
    
    // 指针类型: 指向的类型
    std::shared_ptr<TypeDesc> baseType = nullptr;
    
    // 数组类型: 元素类型和大小
    std::size_t arraySize = 0;
    
    // 函数类型: 返回类型和参数
    std::shared_ptr<TypeDesc> returnType = nullptr;
    std::vector<std::shared_ptr<ParamInfo>> params;
    bool hasVarArgs = false;  // 是否有可变参数 ...
    
    // 结构体/联合体/枚举名
    std::string tagName;
    
    // 类型限定符
    TypeQualifiers qualifiers;
    
    // 存储类
    StorageClass storageClass = StorageClass::None;
    
    // 声明节点引用（用于后续处理）
    const ast::ASTNode* declNode = nullptr;

    TypeDesc() = default;

    // 创建基础类型
    static std::shared_ptr<TypeDesc> createBuiltin(BuiltinType bt,
        TypeQualifiers quals = {}, StorageClass sc = StorageClass::None);
    
    // 创建指针类型
    static std::shared_ptr<TypeDesc> createPointer(
        std::shared_ptr<TypeDesc> base,
        TypeQualifiers quals = {});
    
    // 创建数组类型
    static std::shared_ptr<TypeDesc> createArray(
        std::shared_ptr<TypeDesc> elem, std::size_t size);
    
    // 创建函数类型
    static std::shared_ptr<TypeDesc> createFunction(
        std::shared_ptr<TypeDesc> ret,
        std::vector<std::shared_ptr<ParamInfo>> params,
        bool varArgs = false);
    
    // 创建结构体/联合体/枚举类型
    static std::shared_ptr<TypeDesc> createTag(
        TypeCategory cat, const std::string& name);

    bool isIntegerType() const;
    
    bool isFloatType() const;
    
    bool isArithmeticType() const;
    
    bool isPointerType() const;
    
    bool isArrayType() const;
    
    bool isFunctionType() const;
    
    bool isVoidType() const;
    
    std::shared_ptr<TypeDesc> getElementType() const;
    
    std::size_t getSize() const;
    
    std::size_t getAlignment() const;
};

/* ============================================================
 * Symbol
 * ============================================================ */
struct Symbol {
    std::string name;
    SymbolKind kind;
    std::shared_ptr<TypeDesc> type;
    const ast::ASTNode *declNode = nullptr;
    // 用于 IRBuilder 存储对应的 Koopa 地址/值
    rcc::ir::Value irValue = nullptr;
    // 存储函数对象（如果符号是一个函数）
    rcc::ir::Function irFunction = nullptr;
};

/* ============================================================
 * Tag symbol (struct / union / enum)
 * ============================================================ */
struct TagSymbol {
    std::string name;
    TagKind kind;
    std::shared_ptr<TypeDesc> type;
    const ast::ASTNode *declNode = nullptr;
};

/* ============================================================
 * Scope
 * ============================================================ */
class Scope {
public:
    Scope(Scope *parent = nullptr);
    
    bool insert(const std::shared_ptr<Symbol> &sym);
    bool insertTag(const std::shared_ptr<TagSymbol> &sym);
    
    std::shared_ptr<Symbol> lookupLocal(const std::string &name);
    std::shared_ptr<TagSymbol> lookupTagLocal(const std::string &name);
    
    std::shared_ptr<Symbol> lookup(const std::string &name);
    std::shared_ptr<TagSymbol> lookupTag(const std::string &name);
    
    Scope *parent() const { return parent_; }

private:
    Scope *parent_ = nullptr;
    std::unordered_map<std::string, std::shared_ptr<Symbol>> objects_;
    std::unordered_map<std::string, std::shared_ptr<TagSymbol>> tags_;
};

/* ============================================================
 * Symbol table (scope stack)
 * ============================================================ */
class SymbolTable {
public:
    SymbolTable();
    
    void enterScope();
    void leaveScope();
    Scope *currentScope();
    
    bool define(const std::shared_ptr<Symbol> &sym);
    bool defineTag(const std::shared_ptr<TagSymbol> &sym);
    
    std::shared_ptr<Symbol> lookup(const std::string &name);
    std::shared_ptr<TagSymbol> lookupTag(const std::string &name);

private:
    std::vector<std::unique_ptr<Scope>> scopes_;
    Scope *current_ = nullptr;
};

} // namespace rcc::symtab