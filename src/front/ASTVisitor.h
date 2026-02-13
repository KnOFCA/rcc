#pragma once

#include "AST.h"
#include <memory>

namespace rcc::front {

/**
 * ASTVisitor
 *
 * 提供一个可继承的访问者框架：
 * - 一组 `visit(...)` 虚方法，可由子类重写
 * - 默认实现执行对常见子节点的递归遍历
 */
class ASTVisitor {
  public:
    virtual ~ASTVisitor() = default;

    // 通用分发点，基类实现使用 dynamic_pointer_cast 做类型分发
    virtual void visit(const ast::AST &node);

    // 翻译单元
    virtual void visitTranslationUnit(const std::shared_ptr<ast::TranslationUnit> &tu);

    // 声明 / 定义
    virtual void visitDeclaration(const std::shared_ptr<ast::Declaration> &decl);
    virtual void visitFunctionDef(const std::shared_ptr<ast::FunctionDef> &fdef);

    // 语句层次
    virtual void visitStmt(const std::shared_ptr<ast::Stmt> &stmt);
    virtual void visitCompoundStmt(const std::shared_ptr<ast::CompoundStmt> &comp);

    // 表达式层次
    virtual void visitExpr(const std::shared_ptr<ast::Expr> &expr);
    virtual void visitIdExpr(const std::shared_ptr<ast::IdExpr> &id);
    virtual void visitLiteralExpr(const std::shared_ptr<ast::LiteralExpr> &lit);
    virtual void visitUnaryExpr(const std::shared_ptr<ast::UnaryExpr> &un);
    virtual void visitBinaryExpr(const std::shared_ptr<ast::BinaryExpr> &bin);
    virtual void visitConditionalExpr(const std::shared_ptr<ast::ConditionalExpr> &ce);
    virtual void visitCallExpr(const std::shared_ptr<ast::CallExpr> &call);
    virtual void visitPostfixExpr(const std::shared_ptr<ast::PostfixExpr> &post);

    // 可扩展部分
    virtual void visitDeclarator(const std::shared_ptr<ast::Declarator> &decl);
    virtual void visitTypeName(const std::shared_ptr<ast::TypeName> &tn);
};

} // namespace rcc::front

