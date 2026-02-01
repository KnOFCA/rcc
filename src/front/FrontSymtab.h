#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>

#include "AST.h"

namespace rcc {

/* ============================================================
 *  Symbol kind
 * ============================================================ */

enum class SymbolKind {
    Object,     // 变量
    Function,   // 函数
    Typedef,    // typedef 名
    EnumConst   // 枚举常量
};

enum class TagKind {
    Struct,
    Union,
    Enum
};

/* ============================================================
 *  Type placeholder
 *
 *  先不真正建类型系统，只保存 declarator + specifier，
 *  你后面可以再加真正的 Type 结构。
 * ============================================================ */

struct TypeDesc {
    std::shared_ptr<ast::DeclSpec> specs;
    std::shared_ptr<ast::Declarator> declarator;
};

/* ============================================================
 *  Symbol
 * ============================================================ */

struct Symbol {
    std::string name;
    SymbolKind kind;

    TypeDesc type;

    const ast::ASTNode *declNode = nullptr;
};

/* ============================================================
 *  Tag symbol (struct / union / enum)
 * ============================================================ */

struct TagSymbol {
    std::string name;
    TagKind kind;

    const ast::ASTNode *declNode = nullptr;
};

/* ============================================================
 *  Scope
 * ============================================================ */

class Scope {
public:
    Scope(Scope *parent = nullptr);

    bool insert(const std::shared_ptr<Symbol> &sym);
    bool insertTag(const std::shared_ptr<TagSymbol> &sym);

    std::shared_ptr<Symbol>    lookupLocal(const std::string &name);
    std::shared_ptr<TagSymbol> lookupTagLocal(const std::string &name);

    std::shared_ptr<Symbol>    lookup(const std::string &name);
    std::shared_ptr<TagSymbol> lookupTag(const std::string &name);

    Scope *parent() const { return parent_; }

private:
    Scope *parent_ = nullptr;

    std::unordered_map<std::string, std::shared_ptr<Symbol>> objects_;
    std::unordered_map<std::string, std::shared_ptr<TagSymbol>> tags_;
};

/* ============================================================
 *  Symbol table (scope stack)
 * ============================================================ */

class SymbolTable {
public:
    SymbolTable();

    void enterScope();
    void leaveScope();

    Scope *currentScope();

    bool define(const std::shared_ptr<Symbol> &sym);
    bool defineTag(const std::shared_ptr<TagSymbol> &sym);

    std::shared_ptr<Symbol>    lookup(const std::string &name);
    std::shared_ptr<TagSymbol> lookupTag(const std::string &name);

private:
    std::vector<std::unique_ptr<Scope>> scopes_;
    Scope *current_ = nullptr;
};

} // namespace rcc
