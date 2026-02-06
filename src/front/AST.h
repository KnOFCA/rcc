#pragma once

#include <memory>
#include <string>
#include <vector>

namespace ast {

/* ============================================================
 *  Base
 * ============================================================ */

struct ASTNode {
    virtual ~ASTNode() = default;
};

using AST = std::shared_ptr<ASTNode>;

/* ============================================================
 *  Translation unit / external
 * ============================================================ */

struct ExternalDecl : ASTNode {
};

struct TranslationUnit : ASTNode {
    std::vector<std::shared_ptr<ExternalDecl>> externalDecls;
};

/* ============================================================
 *  Declaration specifiers
 * ============================================================ */

struct DeclSpecifier : ASTNode {
    enum class Kind {
        StorageClass,
        TypeSpecifier,
        TypeQualifier,
        NONE
    };

    Kind kind;

protected:
    explicit DeclSpecifier(Kind k) : kind(k) {}
    explicit DeclSpecifier() : kind(Kind::NONE) {}
};

struct StorageClassSpec : DeclSpecifier {

    enum class Class {
        Auto,
        Register,
        Static,
        Extern,
        Typedef,
        NONE
    };

    Class storageClass;

    explicit StorageClassSpec(Class c)
        : DeclSpecifier(Kind::StorageClass), storageClass(c) {}
    explicit StorageClassSpec()
        : DeclSpecifier(Kind::StorageClass), storageClass(Class::NONE) {}
};

struct TypeQualifierSpec : DeclSpecifier {

    enum class Qualifier {
        Const,
        Volatile,
        Restrict,
        Atomic,
        NONE
    };

    Qualifier qualifier;

    explicit TypeQualifierSpec(Qualifier q)
        : DeclSpecifier(Kind::TypeQualifier), qualifier(q) {}
    explicit TypeQualifierSpec()
        : DeclSpecifier(Kind::TypeQualifier), qualifier(Qualifier::NONE) {}
};

struct TypeSpecifier : DeclSpecifier {

    enum class Category {
        Builtin,
        Struct,
        Union,
        Enum,
        NONE
    };

    Category category;

protected:
    explicit TypeSpecifier(Category c)
        : DeclSpecifier(Kind::TypeSpecifier), category(c) {}
    explicit TypeSpecifier()
        : DeclSpecifier(Kind::TypeSpecifier), category(Category::NONE) {}

};

struct BuiltinTypeSpec : TypeSpecifier {

    enum class Builtin {
        Void,
        Char,
        Short,
        Int,
        Long,
        Float,
        Double,
        Signed,
        Unsigned,
        Bool,
        NONE
    };

    Builtin builtin;

    explicit BuiltinTypeSpec(Builtin b)
        : TypeSpecifier(Category::Builtin), builtin(b) {}
    explicit BuiltinTypeSpec()
        : TypeSpecifier(Category::Builtin), builtin(Builtin::NONE) {}

};

struct StructTypeSpec : TypeSpecifier {

    std::string name; // 可能为空
    std::vector<std::shared_ptr<ASTNode>> fields;

    explicit StructTypeSpec()
        : TypeSpecifier(Category::Struct) {}
};

struct UnionTypeSpec : TypeSpecifier {

    std::string name;
    std::vector<std::shared_ptr<ASTNode>> fields;

    explicit UnionTypeSpec()
        : TypeSpecifier(Category::Union) {}
};

struct EnumTypeSpec : TypeSpecifier {

    std::string name;

    struct Enumerator {
        std::string name;
        std::shared_ptr<ASTNode> value; // constantExpression
    };

    std::vector<Enumerator> enumerators;

    explicit EnumTypeSpec()
        : TypeSpecifier(Category::Enum) {}
};

struct DeclSpec : ASTNode {
    std::vector<std::shared_ptr<DeclSpecifier>> specs;
};

/* ============================================================
 *  Declarators
 * ============================================================ */

struct Pointer : ASTNode {
    std::vector<std::shared_ptr<TypeQualifierSpec>> qualifiers;   // const / volatile / restrict / atomic
    std::shared_ptr<Pointer> next;
};

struct DirectDeclarator : ASTNode {
};

struct DDIdentifier : DirectDeclarator {
    std::string name;
};

struct DDParen : DirectDeclarator {
    AST inner;   // Declarator
};

struct ParameterDecl : ASTNode {
    std::shared_ptr<DeclSpec> specs;
    AST declarator;
};

struct DDCall : DirectDeclarator {
    AST base; // DirectDeclarator
    std::vector<std::shared_ptr<ParameterDecl>> params;
};

struct Declarator : ASTNode {
    std::shared_ptr<Pointer> pointer;
    std::shared_ptr<DirectDeclarator> direct;
};

/* ============================================================
 *  Initializers
 * ============================================================ */

struct Initializer : ASTNode {
};

struct ExprInitializer : Initializer {
    AST expr;
};

struct InitList : Initializer {
    std::vector<std::shared_ptr<Initializer>> elements;
};

/* ============================================================
 *  Declarations / definitions
 * ============================================================ */

struct InitDeclarator : ASTNode {
    std::shared_ptr<Declarator> declarator;
    std::shared_ptr<Initializer> initializer; // Expr or initializer-list
};

struct Declaration : ExternalDecl {
    std::shared_ptr<DeclSpec> specs;
    std::vector<std::shared_ptr<InitDeclarator>> initDeclarators;
};

struct FunctionDef : ExternalDecl {
    std::shared_ptr<DeclSpec> specs;
    std::shared_ptr<Declarator> declarator;
    AST body; // CompoundStmt
};

/* ============================================================
 *  Statements
 * ============================================================ */

struct Stmt : ASTNode {
};

struct BlockItem : ASTNode {
};

struct DeclItem : BlockItem {
    std::shared_ptr<Declaration> decl;
};

struct StmtItem : BlockItem {
    std::shared_ptr<Stmt> stmt;
};

struct CompoundStmt : Stmt {
    std::vector<std::shared_ptr<BlockItem>> items;
};

/* ---------- labeled ---------- */

struct LabeledStmt : Stmt {
};

struct LabelStmt : LabeledStmt {
    std::string label;
    std::shared_ptr<Stmt> stmt;
};

struct CaseStmt : LabeledStmt {
    AST expr;
    std::shared_ptr<Stmt> stmt;
};

struct DefaultStmt : LabeledStmt {
    std::shared_ptr<Stmt> stmt;
};

/* ---------- expression stmt ---------- */

struct ExprStmt : Stmt {
    AST expr; // may be null
};

/* ---------- selection ---------- */

struct IfStmt : Stmt {
    AST cond;
    std::shared_ptr<Stmt> thenStmt;
    std::shared_ptr<Stmt> elseStmt;
};

struct SwitchStmt : Stmt {
    AST expr;
    std::shared_ptr<Stmt> stmt;
};

/* ---------- iteration ---------- */

struct WhileStmt : Stmt {
    AST cond;
    std::shared_ptr<Stmt> body;
};

struct DoWhileStmt : Stmt {
    std::shared_ptr<Stmt> body;
    AST cond;
};

struct ForStmt : Stmt {
    std::shared_ptr<Stmt> init;  // expressionStatement
    std::shared_ptr<Stmt> cond;  // expressionStatement
    AST iter;                    // expression?
    std::shared_ptr<Stmt> body;
};

/* ---------- jump ---------- */

struct GotoStmt : Stmt {
    std::string label;
};

struct ContinueStmt : Stmt {
};

struct BreakStmt : Stmt {
};

struct ReturnStmt : Stmt {
    AST expr;
};

/* ============================================================
 *  Expressions
 * ============================================================ */

struct Expr : ASTNode {
};

struct IdExpr : Expr {
    std::string name;
};

struct LiteralExpr : Expr {
    std::string value;
};

struct UnaryExpr : Expr {
    std::string op;
    AST operand;
};

struct BinaryExpr : Expr {
    std::string op;
    AST lhs;
    AST rhs;
};

struct ConditionalExpr : Expr {
    AST cond;
    AST thenExpr;
    AST elseExpr;
};

struct CallExpr : Expr {
    AST callee;
    std::vector<AST> args;
};

struct PostfixExpr : Expr {
    std::string op; // ++ or --
    AST expr;
};

/* ============================================================
 *  Struct / Union / Enum
 * ============================================================ */

struct StructDecl : ASTNode {
    std::shared_ptr<DeclSpec> specs;
    std::vector<std::shared_ptr<Declarator>> declarators;
};

struct StructSpec : ASTNode {
    std::string name;
    std::vector<std::shared_ptr<StructDecl>> members;
};

struct UnionSpec : ASTNode {
    std::string name;
    std::vector<std::shared_ptr<StructDecl>> members;
};

struct Enumerator : ASTNode {
    std::string name;
    AST value;
};

struct EnumSpec : ASTNode {
    std::string name;
    std::vector<std::shared_ptr<Enumerator>> enumerators;
};

} // namespace AST
