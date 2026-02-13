#include "ASTVisitor.h"

using namespace rcc::front;

void ASTVisitor::visit(const ast::AST &node) {
    if (!node) return;

    if (auto tu = std::dynamic_pointer_cast<ast::TranslationUnit>(node)) {
    visitTranslationUnit(tu);
    return;
    }
    if (auto decl = std::dynamic_pointer_cast<ast::Declaration>(node)) {
    visitDeclaration(decl);
    return;
    }
    if (auto fdef = std::dynamic_pointer_cast<ast::FunctionDef>(node)) {
    visitFunctionDef(fdef);
    return;
    }
    if (auto stmt = std::dynamic_pointer_cast<ast::Stmt>(node)) {
    visitStmt(stmt);
    return;
    }
    if (auto expr = std::dynamic_pointer_cast<ast::Expr>(node)) {
    visitExpr(expr);
    return;
    }
    // 未专门处理的节点类型：子类可以 override visit(node)
}

void ASTVisitor::visitTranslationUnit(const std::shared_ptr<ast::TranslationUnit> &tu) {
    if (!tu) return;
    for (auto &ed : tu->externalDecls) visit(ed);
}

// 声明 / 定义
void ASTVisitor::visitDeclaration(const std::shared_ptr<ast::Declaration> &decl) {
    if (!decl) return;
    if (decl->specs) {
    // 子类可覆盖以处理类型/存储类信息
    }
    for (auto &init : decl->initDeclarators) {
    if (!init) continue;
    visit(std::static_pointer_cast<ast::ASTNode>(init->declarator));
    if (init->initializer) visit(std::static_pointer_cast<ast::ASTNode>(init->initializer));
    }
}

void ASTVisitor::visitFunctionDef(const std::shared_ptr<ast::FunctionDef> &fdef) {
    if (!fdef) return;
    if (fdef->declarator) visit(std::static_pointer_cast<ast::ASTNode>(fdef->declarator));
    if (fdef->body) visit(fdef->body);
}

// 语句层次
void ASTVisitor::visitStmt(const std::shared_ptr<ast::Stmt> &stmt) {
    if (!stmt) return;
    if (auto comp = std::dynamic_pointer_cast<ast::CompoundStmt>(stmt)) {
    visitCompoundStmt(comp);
    return;
    }
    if (auto exprStmt = std::dynamic_pointer_cast<ast::ExprStmt>(stmt)) {
    if (exprStmt->expr) visit(exprStmt->expr);
    return;
    }
    if (auto ifs = std::dynamic_pointer_cast<ast::IfStmt>(stmt)) {
    if (ifs->cond) visit(ifs->cond);
    if (ifs->thenStmt) visit(ifs->thenStmt);
    if (ifs->elseStmt) visit(ifs->elseStmt);
    return;
    }
    if (auto ws = std::dynamic_pointer_cast<ast::WhileStmt>(stmt)) {
    if (ws->cond) visit(ws->cond);
    if (ws->body) visit(ws->body);
    return;
    }
    if (auto rs = std::dynamic_pointer_cast<ast::ReturnStmt>(stmt)) {
    if (rs->expr) visit(rs->expr);
    return;
    }
    if (auto forS = std::dynamic_pointer_cast<ast::ForStmt>(stmt)) {
    if (forS->init) visit(forS->init);
    if (forS->cond) visit(forS->cond);
    if (forS->iter) visit(forS->iter);
    if (forS->body) visit(forS->body);
    return;
    }
    // 其它语句类型可由子类处理或在这里补充
}

void ASTVisitor::visitCompoundStmt(const std::shared_ptr<ast::CompoundStmt> &comp) {
    if (!comp) return;
    for (auto &item : comp->items) {
    if (!item) continue;
    if (auto di = std::dynamic_pointer_cast<ast::DeclItem>(item)) {
        if (di->decl) visit(std::static_pointer_cast<ast::ASTNode>(di->decl));
    } else if (auto si = std::dynamic_pointer_cast<ast::StmtItem>(item)) {
        if (si->stmt) visit(si->stmt);
    }
    }
}

// 表达式层次
void ASTVisitor::visitExpr(const std::shared_ptr<ast::Expr> &expr) {
    if (!expr) return;
    if (auto id = std::dynamic_pointer_cast<ast::IdExpr>(expr)) { visitIdExpr(id); return; }
    if (auto lit = std::dynamic_pointer_cast<ast::LiteralExpr>(expr)) { visitLiteralExpr(lit); return; }
    if (auto un = std::dynamic_pointer_cast<ast::UnaryExpr>(expr)) { visitUnaryExpr(un); return; }
    if (auto bin = std::dynamic_pointer_cast<ast::BinaryExpr>(expr)) { visitBinaryExpr(bin); return; }
    if (auto cond = std::dynamic_pointer_cast<ast::ConditionalExpr>(expr)) { visitConditionalExpr(cond); return; }
    if (auto call = std::dynamic_pointer_cast<ast::CallExpr>(expr)) { visitCallExpr(call); return; }
    if (auto post = std::dynamic_pointer_cast<ast::PostfixExpr>(expr)) { visitPostfixExpr(post); return; }
}

void ASTVisitor::visitIdExpr(const std::shared_ptr<ast::IdExpr> &id) {
    (void)id;
}

void ASTVisitor::visitLiteralExpr(const std::shared_ptr<ast::LiteralExpr> &lit) {
    (void)lit;
}

void ASTVisitor::visitUnaryExpr(const std::shared_ptr<ast::UnaryExpr> &un) {
    if (!un) return;
    if (un->operand) visit(un->operand);
}

void ASTVisitor::visitBinaryExpr(const std::shared_ptr<ast::BinaryExpr> &bin) {
    if (!bin) return;
    if (bin->lhs) visit(bin->lhs);
    if (bin->rhs) visit(bin->rhs);
}

void ASTVisitor::visitConditionalExpr(const std::shared_ptr<ast::ConditionalExpr> &ce) {
    if (!ce) return;
    if (ce->cond) visit(ce->cond);
    if (ce->thenExpr) visit(ce->thenExpr);
    if (ce->elseExpr) visit(ce->elseExpr);
}

void ASTVisitor::visitCallExpr(const std::shared_ptr<ast::CallExpr> &call) {
    if (!call) return;
    if (call->callee) visit(call->callee);
    for (auto &a : call->args) if (a) visit(a);
}

void ASTVisitor::visitPostfixExpr(const std::shared_ptr<ast::PostfixExpr> &post) {
    if (!post) return;
    if (post->expr) visit(post->expr);
}

// 可扩展：对 Declarator / TypeName / Initializer 等提供钩子
void ASTVisitor::visitDeclarator(const std::shared_ptr<ast::Declarator> &decl) {
    if (!decl) return;
    if (decl->pointer) {
    // pointer 的字段可由子类处理
    }
    if (decl->direct) {
    // direct declarator 具体类型繁多，子类可按需实现
    }
}

void ASTVisitor::visitTypeName(const std::shared_ptr<ast::TypeName> &tn) {
    if (!tn) return;
    if (tn->specs) {
    // 类型说明符列表；子类可解析
    }
    if (tn->abstractDecl) visit(tn->abstractDecl);
}