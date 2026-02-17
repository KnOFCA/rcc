#pragma once

#include "AST.h"
#include "ASTVisitor.h"
#include "FrontSymtab.h"

#include <memory.h>

namespace rcc::front {

class SymtabCollector : public ASTVisitor<SymtabCollector> {
  public:
    ///
    /// visit method.
    ///
    void visitTranslationUnit(const std::shared_ptr<ast::TranslationUnit> &tu);

    void visitDeclaration(const std::shared_ptr<ast::Declaration> &decl);
    void visitFunctionDef(const std::shared_ptr<ast::FunctionDef> &fdef);

    void visitStmt(const std::shared_ptr<ast::Stmt> &stmt);
    void visitCompoundStmt(const std::shared_ptr<ast::CompoundStmt> &comp);

    void visitExpr(const std::shared_ptr<ast::Expr> &expr);
    void visitIdExpr(const std::shared_ptr<ast::IdExpr> &id);
    void visitLiteralExpr(const std::shared_ptr<ast::LiteralExpr> &lit);
    void visitUnaryExpr(const std::shared_ptr<ast::UnaryExpr> &un);
    void visitBinaryExpr(const std::shared_ptr<ast::BinaryExpr> &bin);
    void visitConditionalExpr(const std::shared_ptr<ast::ConditionalExpr> &ce);
    void visitCallExpr(const std::shared_ptr<ast::CallExpr> &call);
    void visitPostfixExpr(const std::shared_ptr<ast::PostfixExpr> &post);

    void visitDeclarator(const std::shared_ptr<ast::Declarator> &decl);
    void visitTypeName(const std::shared_ptr<ast::TypeName> &tn);

  private:
    SymbolTable table;
};
}