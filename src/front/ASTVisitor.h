#pragma once

#include "AST.h"
#include <memory>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace rcc::front {

// ============================================================================
// RecursiveASTVisitor - 参考 LLVM 的设计
// ============================================================================
// CRTP 基类，派生类应继承为:
//   class MyVisitor : public RecursiveASTVisitor<MyVisitor> { ... };
//
// 核心方法:
//   - TraverseXXX(Node*): 控制遍历流程，返回 bool (true 继续，false 终止)
//   - VisitXXX(Node*): 访问特定节点时的处理
//   - WalkUpFromXXX(Node*): 沿继承层次向上调用 Visit
//
// 派生类可以:
//   - 重写 VisitXXX() 来自定义节点处理
//   - 重写 TraverseXXX() 来自定义遍历行为（如跳过子节点）
// ============================================================================

template <class Derived>
class RecursiveASTVisitor {
protected:
    Derived& derived() { return static_cast<Derived&>(*this); }
    const Derived& derived() const { return static_cast<const Derived&>(*this); }

public:
    RecursiveASTVisitor() = default;
    virtual ~RecursiveASTVisitor() = default;

    // ========================================================================
    // 主入口 - 接受 shared_ptr
    // ========================================================================
    template<typename T>
    bool visit(const std::shared_ptr<T>& node) {
        if (!node) return true;
        return derived().TraverseASTNode(
            std::static_pointer_cast<ast::ASTNode>(node));
    }

    // ========================================================================
    // ASTNode 入口点 - 使用 WalkUp 模式
    // ========================================================================
    bool TraverseASTNode(const std::shared_ptr<ast::ASTNode>& node) {
        if (!node) return true;
        
        // 使用 WalkUp 模式，从最具体的类型开始匹配
        // 派生类可以重写 WalkUpFromASTNode 来拦截所有节点
        return derived().WalkUpFromASTNode(node);
    }

    bool WalkUpFromASTNode(const std::shared_ptr<ast::ASTNode>& node) {
        // 尝试向下转型到具体类型
        // 按照类型层次结构进行匹配
        
        // TranslationUnit
        if (auto tu = std::dynamic_pointer_cast<ast::TranslationUnit>(node))
            return derived().WalkUpFromTranslationUnit(tu);
        
        // ExternalDecl (及其派生)
        if (auto ed = std::dynamic_pointer_cast<ast::ExternalDecl>(node))
            return derived().WalkUpFromExternalDecl(ed);
        
        // Stmt (及其派生)
        if (auto stmt = std::dynamic_pointer_cast<ast::Stmt>(node))
            return derived().WalkUpFromStmt(stmt);
        
        // Expr (及其派生)
        if (auto expr = std::dynamic_pointer_cast<ast::Expr>(node))
            return derived().WalkUpFromExpr(expr);
        
        // Declarator
        if (auto decl = std::dynamic_pointer_cast<ast::Declarator>(node))
            return derived().WalkUpFromDeclarator(decl);
        
        // DeclSpec
        if (auto ds = std::dynamic_pointer_cast<ast::DeclSpec>(node))
            return derived().WalkUpFromDeclSpec(ds);
        
        // DeclSpecifier (及其派生)
        if (auto dspec = std::dynamic_pointer_cast<ast::DeclSpecifier>(node))
            return derived().WalkUpFromDeclSpecifier(dspec);
        
        // TypeName
        if (auto tn = std::dynamic_pointer_cast<ast::TypeName>(node))
            return derived().WalkUpFromTypeName(tn);
        
        // Initializer
        if (auto init = std::dynamic_pointer_cast<ast::Initializer>(node))
            return derived().WalkUpFromInitializer(init);
        
        // BlockItem
        if (auto bi = std::dynamic_pointer_cast<ast::BlockItem>(node))
            return derived().WalkUpFromBlockItem(bi);
        
        // InitDeclarator
        if (auto id = std::dynamic_pointer_cast<ast::InitDeclarator>(node))
            return derived().WalkUpFromInitDeclarator(id);
        
        // Pointer
        if (auto ptr = std::dynamic_pointer_cast<ast::Pointer>(node))
            return derived().WalkUpFromPointer(ptr);
        
        // DirectDeclarator
        if (auto dd = std::dynamic_pointer_cast<ast::DirectDeclarator>(node))
            return derived().WalkUpFromDirectDeclarator(dd);
        
        // AbstractDeclarator
        if (auto ad = std::dynamic_pointer_cast<ast::AbstractDeclarator>(node))
            return derived().WalkUpFromAbstractDeclarator(ad);
        
        // DirectAbstractDeclarator
        if (auto dad = std::dynamic_pointer_cast<ast::DirectAbstractDeclarator>(node))
            return derived().WalkUpFromDirectAbstractDeclarator(dad);
        
        // ParameterDecl
        if (auto pd = std::dynamic_pointer_cast<ast::ParameterDecl>(node))
            return derived().WalkUpFromParameterDecl(pd);
        
        // StructDecl / StructSpec / UnionSpec / EnumSpec / Enumerator
        if (auto sd = std::dynamic_pointer_cast<ast::StructDecl>(node))
            return derived().WalkUpFromStructDecl(sd);
        if (auto ss = std::dynamic_pointer_cast<ast::StructSpec>(node))
            return derived().WalkUpFromStructSpec(ss);
        if (auto us = std::dynamic_pointer_cast<ast::UnionSpec>(node))
            return derived().WalkUpFromUnionSpec(us);
        if (auto es = std::dynamic_pointer_cast<ast::EnumSpec>(node))
            return derived().WalkUpFromEnumSpec(es);
        if (auto e = std::dynamic_pointer_cast<ast::Enumerator>(node))
            return derived().WalkUpFromEnumerator(e);
        
        // 默认处理
        return derived().VisitASTNode(node);
    }

    bool VisitASTNode(const std::shared_ptr<ast::ASTNode>& node) {
        (void)node;
        return true;
    }

    // ========================================================================
    // TranslationUnit
    // ========================================================================
    bool WalkUpFromTranslationUnit(const std::shared_ptr<ast::TranslationUnit>& tu) {
        if (!derived().VisitTranslationUnit(tu)) return false;
        return true;
    }

    bool TraverseTranslationUnit(const std::shared_ptr<ast::TranslationUnit>& tu) {
        if (!tu) return true;
        for (auto& ed : tu->externalDecls) {
            if (!visit(ed)) return false;
        }
        return true;
    }

    bool VisitTranslationUnit(const std::shared_ptr<ast::TranslationUnit>& tu) {
        if (!tu) return true;
        return derived().TraverseTranslationUnit(tu);
    }

    // ========================================================================
    // ExternalDecl (基类)
    // ========================================================================
    bool WalkUpFromExternalDecl(const std::shared_ptr<ast::ExternalDecl>& ed) {
        // 先检查具体类型
        if (auto decl = std::dynamic_pointer_cast<ast::Declaration>(ed))
            return derived().WalkUpFromDeclaration(decl);
        if (auto fdef = std::dynamic_pointer_cast<ast::FunctionDef>(ed))
            return derived().WalkUpFromFunctionDef(fdef);
        
        // 回退到基类 Visit
        return derived().VisitExternalDecl(ed);
    }

    bool VisitExternalDecl(const std::shared_ptr<ast::ExternalDecl>& ed) {
        (void)ed;
        return true;
    }

    // ========================================================================
    // Declaration
    // ========================================================================
    bool WalkUpFromDeclaration(const std::shared_ptr<ast::Declaration>& decl) {
        if (!derived().VisitDeclaration(decl)) return false;
        return true;
    }

    bool TraverseDeclaration(const std::shared_ptr<ast::Declaration>& decl) {
        if (!decl) return true;
        if (decl->specs && !visit(decl->specs)) return false;
        for (auto& init : decl->initDeclarators) {
            if (!visit(init)) return false;
        }
        return true;
    }

    bool VisitDeclaration(const std::shared_ptr<ast::Declaration>& decl) {
        if (!decl) return true;
        std::cout << "Declaration (initDeclarators: " << decl->initDeclarators.size() << ")" << std::endl;
        return derived().TraverseDeclaration(decl);
    }

    // ========================================================================
    // FunctionDef
    // ========================================================================
    bool WalkUpFromFunctionDef(const std::shared_ptr<ast::FunctionDef>& fdef) {
        if (!derived().VisitFunctionDef(fdef)) return false;
        return true;
    }

    bool TraverseFunctionDef(const std::shared_ptr<ast::FunctionDef>& fdef) {
        if (!fdef) return true;
        if (fdef->specs && !visit(fdef->specs)) return false;
        if (fdef->declarator && !visit(fdef->declarator)) return false;
        if (fdef->body && !visit(fdef->body)) return false;
        return true;
    }

    bool VisitFunctionDef(const std::shared_ptr<ast::FunctionDef>& fdef) {
        if (!fdef) return true;
        std::cout << "FunctionDef" << std::endl;
        return derived().TraverseFunctionDef(fdef);
    }

    // ========================================================================
    // Stmt (基类)
    // ========================================================================
    bool WalkUpFromStmt(const std::shared_ptr<ast::Stmt>& stmt) {
        // CompoundStmt
        if (auto comp = std::dynamic_pointer_cast<ast::CompoundStmt>(stmt))
            return derived().WalkUpFromCompoundStmt(comp);
        // LabeledStmt 系列
        if (auto ls = std::dynamic_pointer_cast<ast::LabeledStmt>(stmt))
            return derived().WalkUpFromLabeledStmt(ls);
        // ExprStmt
        if (auto es = std::dynamic_pointer_cast<ast::ExprStmt>(stmt))
            return derived().WalkUpFromExprStmt(es);
        // Selection
        if (auto ifs = std::dynamic_pointer_cast<ast::IfStmt>(stmt))
            return derived().WalkUpFromIfStmt(ifs);
        if (auto ss = std::dynamic_pointer_cast<ast::SwitchStmt>(stmt))
            return derived().WalkUpFromSwitchStmt(ss);
        // Iteration
        if (auto ws = std::dynamic_pointer_cast<ast::WhileStmt>(stmt))
            return derived().WalkUpFromWhileStmt(ws);
        if (auto dws = std::dynamic_pointer_cast<ast::DoWhileStmt>(stmt))
            return derived().WalkUpFromDoWhileStmt(dws);
        if (auto fs = std::dynamic_pointer_cast<ast::ForStmt>(stmt))
            return derived().WalkUpFromForStmt(fs);
        // Jump
        if (auto gs = std::dynamic_pointer_cast<ast::GotoStmt>(stmt))
            return derived().WalkUpFromGotoStmt(gs);
        if (auto cs = std::dynamic_pointer_cast<ast::ContinueStmt>(stmt))
            return derived().WalkUpFromContinueStmt(cs);
        if (auto bs = std::dynamic_pointer_cast<ast::BreakStmt>(stmt))
            return derived().WalkUpFromBreakStmt(bs);
        if (auto rs = std::dynamic_pointer_cast<ast::ReturnStmt>(stmt))
            return derived().WalkUpFromReturnStmt(rs);
        
        return derived().VisitStmt(stmt);
    }

    bool VisitStmt(const std::shared_ptr<ast::Stmt>& stmt) {
        (void)stmt;
        return true;
    }

    // ------------------------------------------------------------------------
    // CompoundStmt
    // ------------------------------------------------------------------------
    bool WalkUpFromCompoundStmt(const std::shared_ptr<ast::CompoundStmt>& comp) {
        if (!derived().VisitCompoundStmt(comp)) return false;
        return true;
    }

    bool TraverseCompoundStmt(const std::shared_ptr<ast::CompoundStmt>& comp) {
        if (!comp) return true;
        for (auto& item : comp->items) {
            if (!visit(item)) return false;
        }
        return true;
    }

    bool VisitCompoundStmt(const std::shared_ptr<ast::CompoundStmt>& comp) {
        if (!comp) return true;
        std::cout << "CompoundStmt (items: " << comp->items.size() << ")" << std::endl;
        return derived().TraverseCompoundStmt(comp);
    }

    // ------------------------------------------------------------------------
    // LabeledStmt (基类)
    // ------------------------------------------------------------------------
    bool WalkUpFromLabeledStmt(const std::shared_ptr<ast::LabeledStmt>& ls) {
        if (auto lbl = std::dynamic_pointer_cast<ast::LabelStmt>(ls))
            return derived().WalkUpFromLabelStmt(lbl);
        if (auto cs = std::dynamic_pointer_cast<ast::CaseStmt>(ls))
            return derived().WalkUpFromCaseStmt(cs);
        if (auto ds = std::dynamic_pointer_cast<ast::DefaultStmt>(ls))
            return derived().WalkUpFromDefaultStmt(ds);
        
        return derived().VisitLabeledStmt(ls);
    }

    bool VisitLabeledStmt(const std::shared_ptr<ast::LabeledStmt>& ls) {
        (void)ls;
        return true;
    }

    bool WalkUpFromLabelStmt(const std::shared_ptr<ast::LabelStmt>& ls) {
        if (!derived().VisitLabelStmt(ls)) return false;
        return true;
    }

    bool TraverseLabelStmt(const std::shared_ptr<ast::LabelStmt>& ls) {
        if (!ls) return true;
        if (ls->stmt && !visit(ls->stmt)) return false;
        return true;
    }

    bool VisitLabelStmt(const std::shared_ptr<ast::LabelStmt>& ls) {
        if (!ls) return true;
        std::cout << "Label: " << ls->label << std::endl;
        return derived().TraverseLabelStmt(ls);
    }

    bool WalkUpFromCaseStmt(const std::shared_ptr<ast::CaseStmt>& cs) {
        if (!derived().VisitCaseStmt(cs)) return false;
        return true;
    }

    bool TraverseCaseStmt(const std::shared_ptr<ast::CaseStmt>& cs) {
        if (!cs) return true;
        if (cs->expr && !visit(cs->expr)) return false;
        if (cs->stmt && !visit(cs->stmt)) return false;
        return true;
    }

    bool VisitCaseStmt(const std::shared_ptr<ast::CaseStmt>& cs) {
        if (!cs) return true;
        std::cout << "Case: " << std::endl;
        return derived().TraverseCaseStmt(cs);
    }

    bool WalkUpFromDefaultStmt(const std::shared_ptr<ast::DefaultStmt>& ds) {
        if (!derived().VisitDefaultStmt(ds)) return false;
        return true;
    }

    bool TraverseDefaultStmt(const std::shared_ptr<ast::DefaultStmt>& ds) {
        if (!ds) return true;
        if (ds->stmt && !visit(ds->stmt)) return false;
        return true;
    }

    bool VisitDefaultStmt(const std::shared_ptr<ast::DefaultStmt>& ds) {
        if (!ds) return true;
        std::cout << "Default: " << std::endl;
        return derived().TraverseDefaultStmt(ds);
    }

    // ------------------------------------------------------------------------
    // ExprStmt
    // ------------------------------------------------------------------------
    bool WalkUpFromExprStmt(const std::shared_ptr<ast::ExprStmt>& es) {
        if (!derived().VisitExprStmt(es)) return false;
        return true;
    }

    bool TraverseExprStmt(const std::shared_ptr<ast::ExprStmt>& es) {
        if (!es) return true;
        if (es->expr && !visit(es->expr)) return false;
        return true;
    }

    bool VisitExprStmt(const std::shared_ptr<ast::ExprStmt>& es) {
        if (!es) return true;
        return derived().TraverseExprStmt(es);
    }

    // ------------------------------------------------------------------------
    // IfStmt
    // ------------------------------------------------------------------------
    bool WalkUpFromIfStmt(const std::shared_ptr<ast::IfStmt>& ifs) {
        if (!derived().VisitIfStmt(ifs)) return false;
        return true;
    }

    bool TraverseIfStmt(const std::shared_ptr<ast::IfStmt>& ifs) {
        if (!ifs) return true;
        if (ifs->cond && !visit(ifs->cond)) return false;
        if (ifs->thenStmt && !visit(ifs->thenStmt)) return false;
        if (ifs->elseStmt && !visit(ifs->elseStmt)) return false;
        return true;
    }

    bool VisitIfStmt(const std::shared_ptr<ast::IfStmt>& ifs) {
        if (!ifs) return true;
        std::cout << "IfStmt (hasElse: " << (ifs->elseStmt ? "true" : "false") << ")" << std::endl;
        return derived().TraverseIfStmt(ifs);
    }

    // ------------------------------------------------------------------------
    // SwitchStmt
    // ------------------------------------------------------------------------
    bool WalkUpFromSwitchStmt(const std::shared_ptr<ast::SwitchStmt>& ss) {
        if (!derived().VisitSwitchStmt(ss)) return false;
        return true;
    }

    bool TraverseSwitchStmt(const std::shared_ptr<ast::SwitchStmt>& ss) {
        if (!ss) return true;
        if (ss->expr && !visit(ss->expr)) return false;
        if (ss->stmt && !visit(ss->stmt)) return false;
        return true;
    }

    bool VisitSwitchStmt(const std::shared_ptr<ast::SwitchStmt>& ss) {
        if (!ss) return true;
        std::cout << "SwitchStmt" << std::endl;
        return derived().TraverseSwitchStmt(ss);
    }

    // ------------------------------------------------------------------------
    // WhileStmt
    // ------------------------------------------------------------------------
    bool WalkUpFromWhileStmt(const std::shared_ptr<ast::WhileStmt>& ws) {
        if (!derived().VisitWhileStmt(ws)) return false;
        return true;
    }

    bool TraverseWhileStmt(const std::shared_ptr<ast::WhileStmt>& ws) {
        if (!ws) return true;
        if (ws->cond && !visit(ws->cond)) return false;
        if (ws->body && !visit(ws->body)) return false;
        return true;
    }

    bool VisitWhileStmt(const std::shared_ptr<ast::WhileStmt>& ws) {
        if (!ws) return true;
        std::cout << "WhileStmt" << std::endl;
        return derived().TraverseWhileStmt(ws);
    }

    // ------------------------------------------------------------------------
    // DoWhileStmt
    // ------------------------------------------------------------------------
    bool WalkUpFromDoWhileStmt(const std::shared_ptr<ast::DoWhileStmt>& dws) {
        if (!derived().VisitDoWhileStmt(dws)) return false;
        return true;
    }

    bool TraverseDoWhileStmt(const std::shared_ptr<ast::DoWhileStmt>& dws) {
        if (!dws) return true;
        if (dws->body && !visit(dws->body)) return false;
        if (dws->cond && !visit(dws->cond)) return false;
        return true;
    }

    bool VisitDoWhileStmt(const std::shared_ptr<ast::DoWhileStmt>& dws) {
        if (!dws) return true;
        std::cout << "DoWhileStmt" << std::endl;
        return derived().TraverseDoWhileStmt(dws);
    }

    // ------------------------------------------------------------------------
    // ForStmt
    // ------------------------------------------------------------------------
    bool WalkUpFromForStmt(const std::shared_ptr<ast::ForStmt>& fs) {
        if (!derived().VisitForStmt(fs)) return false;
        return true;
    }

    bool TraverseForStmt(const std::shared_ptr<ast::ForStmt>& fs) {
        if (!fs) return true;
        if (fs->init && !visit(fs->init)) return false;
        if (fs->cond && !visit(fs->cond)) return false;
        if (fs->iter && !visit(fs->iter)) return false;
        if (fs->body && !visit(fs->body)) return false;
        return true;
    }

    bool VisitForStmt(const std::shared_ptr<ast::ForStmt>& fs) {
        if (!fs) return true;
        std::cout << "ForStmt" << std::endl;
        return derived().TraverseForStmt(fs);
    }

    // ------------------------------------------------------------------------
    // Jump Statements
    // ------------------------------------------------------------------------
    bool WalkUpFromGotoStmt(const std::shared_ptr<ast::GotoStmt>& gs) {
        if (!derived().VisitGotoStmt(gs)) return false;
        return true;
    }

    bool VisitGotoStmt(const std::shared_ptr<ast::GotoStmt>& gs) {
        if (!gs) return true;
        std::cout << "GotoStmt: " << gs->label << std::endl;
        return true;
    }

    bool WalkUpFromContinueStmt(const std::shared_ptr<ast::ContinueStmt>& cs) {
        if (!derived().VisitContinueStmt(cs)) return false;
        return true;
    }

    bool VisitContinueStmt(const std::shared_ptr<ast::ContinueStmt>& cs) {
        if (!cs) return true;
        std::cout << "ContinueStmt" << std::endl;
        return true;
    }

    bool WalkUpFromBreakStmt(const std::shared_ptr<ast::BreakStmt>& bs) {
        if (!derived().VisitBreakStmt(bs)) return false;
        return true;
    }

    bool VisitBreakStmt(const std::shared_ptr<ast::BreakStmt>& bs) {
        if (!bs) return true;
        std::cout << "BreakStmt" << std::endl;
        return true;
    }

    bool WalkUpFromReturnStmt(const std::shared_ptr<ast::ReturnStmt>& rs) {
        if (!derived().VisitReturnStmt(rs)) return false;
        return true;
    }

    bool TraverseReturnStmt(const std::shared_ptr<ast::ReturnStmt>& rs) {
        if (!rs) return true;
        if (rs->expr && !visit(rs->expr)) return false;
        return true;
    }

    bool VisitReturnStmt(const std::shared_ptr<ast::ReturnStmt>& rs) {
        if (!rs) return true;
        std::cout << "ReturnStmt (hasValue: " << (rs->expr ? "true" : "false") << ")" << std::endl;
        return derived().TraverseReturnStmt(rs);
    }

    // ========================================================================
    // Expr (基类)
    // ========================================================================
    bool WalkUpFromExpr(const std::shared_ptr<ast::Expr>& expr) {
        if (auto id = std::dynamic_pointer_cast<ast::IdExpr>(expr))
            return derived().WalkUpFromIdExpr(id);
        if (auto lit = std::dynamic_pointer_cast<ast::LiteralExpr>(expr))
            return derived().WalkUpFromLiteralExpr(lit);
        if (auto un = std::dynamic_pointer_cast<ast::UnaryExpr>(expr))
            return derived().WalkUpFromUnaryExpr(un);
        if (auto bin = std::dynamic_pointer_cast<ast::BinaryExpr>(expr))
            return derived().WalkUpFromBinaryExpr(bin);
        if (auto cond = std::dynamic_pointer_cast<ast::ConditionalExpr>(expr))
            return derived().WalkUpFromConditionalExpr(cond);
        if (auto call = std::dynamic_pointer_cast<ast::CallExpr>(expr))
            return derived().WalkUpFromCallExpr(call);
        if (auto post = std::dynamic_pointer_cast<ast::PostfixExpr>(expr))
            return derived().WalkUpFromPostfixExpr(post);
        if (auto cast = std::dynamic_pointer_cast<ast::CastExpr>(expr))
            return derived().WalkUpFromCastExpr(cast);
        
        return derived().VisitExpr(expr);
    }

    bool VisitExpr(const std::shared_ptr<ast::Expr>& expr) {
        (void)expr;
        return true;
    }

    // ------------------------------------------------------------------------
    // IdExpr
    // ------------------------------------------------------------------------
    bool WalkUpFromIdExpr(const std::shared_ptr<ast::IdExpr>& id) {
        if (!derived().VisitIdExpr(id)) return false;
        return true;
    }

    bool VisitIdExpr(const std::shared_ptr<ast::IdExpr>& id) {
        if (!id) return true;
        std::cout << "IdExpr: " << id->name << std::endl;
        return true;
    }

    // ------------------------------------------------------------------------
    // LiteralExpr
    // ------------------------------------------------------------------------
    bool WalkUpFromLiteralExpr(const std::shared_ptr<ast::LiteralExpr>& lit) {
        if (!derived().VisitLiteralExpr(lit)) return false;
        return true;
    }

    bool VisitLiteralExpr(const std::shared_ptr<ast::LiteralExpr>& lit) {
        if (!lit) return true;
        static const char* typeNames[] = {"INTEGER", "FLOAT", "CHAR", "STRING"};
        std::cout << "LiteralExpr [" << typeNames[static_cast<int>(lit->type)] << "]: " << lit->value << std::endl;
        return true;
    }

    // ------------------------------------------------------------------------
    // UnaryExpr
    // ------------------------------------------------------------------------
    bool WalkUpFromUnaryExpr(const std::shared_ptr<ast::UnaryExpr>& un) {
        if (!derived().VisitUnaryExpr(un)) return false;
        return true;
    }

    bool TraverseUnaryExpr(const std::shared_ptr<ast::UnaryExpr>& un) {
        if (!un) return true;
        if (un->operand && !visit(un->operand)) return false;
        return true;
    }

    bool VisitUnaryExpr(const std::shared_ptr<ast::UnaryExpr>& un) {
        if (!un) return true;
        static const char* opNames[] = {
            "COMMA", "ASSIGN", "PLUSASSIGN", "MINUSASSIGN", "MULASSIGN", "DIVASSIGN", "MODASSIGN",
            "ANDASSIGN", "ORASSIGN", "XORASSIGN", "LSASSIGN", "RSASSIGN",
            "OROR", "ANDAND", "BITOR", "BITXOR", "BITAND", "BITNOT", "LSHIFT", "RSHIFT",
            "EQ", "NEQ", "LT", "GT", "LE", "GE",
            "ADD", "SUB", "MUL", "DIV", "MOD",
            "STAR", "AMP", "PLUSPLUS", "MINUSMINUS", "ARRAY", "NONE"
        };
        std::cout << "UnaryExpr [" << opNames[static_cast<int>(un->op)] << "]" << std::endl;
        return derived().TraverseUnaryExpr(un);
    }

    // ------------------------------------------------------------------------
    // BinaryExpr
    // ------------------------------------------------------------------------
    bool WalkUpFromBinaryExpr(const std::shared_ptr<ast::BinaryExpr>& bin) {
        if (!derived().VisitBinaryExpr(bin)) return false;
        return true;
    }

    bool TraverseBinaryExpr(const std::shared_ptr<ast::BinaryExpr>& bin) {
        if (!bin) return true;
        if (bin->lhs && !visit(bin->lhs)) return false;
        if (bin->rhs && !visit(bin->rhs)) return false;
        return true;
    }

    bool VisitBinaryExpr(const std::shared_ptr<ast::BinaryExpr>& bin) {
        if (!bin) return true;
        static const char* opNames[] = {
            "COMMA", "ASSIGN", "PLUSASSIGN", "MINUSASSIGN", "MULASSIGN", "DIVASSIGN", "MODASSIGN",
            "ANDASSIGN", "ORASSIGN", "XORASSIGN", "LSASSIGN", "RSASSIGN",
            "OROR", "ANDAND", "BITOR", "BITXOR", "BITAND", "LSHIFT", "RSHIFT",
            "EQ", "NEQ", "LT", "GT", "LE", "GE",
            "ADD", "SUB", "MUL", "DIV", "MOD",
            "STAR", "AMP", "PLUSPLUS", "MINUSMINUS", "ARRAY", "NONE"
        };
        std::cout << "BinaryExpr [" << opNames[static_cast<int>(bin->op)] << "]" << std::endl;
        return derived().TraverseBinaryExpr(bin);
    }

    // ------------------------------------------------------------------------
    // ConditionalExpr
    // ------------------------------------------------------------------------
    bool WalkUpFromConditionalExpr(const std::shared_ptr<ast::ConditionalExpr>& ce) {
        if (!derived().VisitConditionalExpr(ce)) return false;
        return true;
    }

    bool TraverseConditionalExpr(const std::shared_ptr<ast::ConditionalExpr>& ce) {
        if (!ce) return true;
        if (ce->cond && !visit(ce->cond)) return false;
        if (ce->thenExpr && !visit(ce->thenExpr)) return false;
        if (ce->elseExpr && !visit(ce->elseExpr)) return false;
        return true;
    }

    bool VisitConditionalExpr(const std::shared_ptr<ast::ConditionalExpr>& ce) {
        if (!ce) return true;
        std::cout << "ConditionalExpr (?:)" << std::endl;
        return derived().TraverseConditionalExpr(ce);
    }

    // ------------------------------------------------------------------------
    // CallExpr
    // ------------------------------------------------------------------------
    bool WalkUpFromCallExpr(const std::shared_ptr<ast::CallExpr>& call) {
        if (!derived().VisitCallExpr(call)) return false;
        return true;
    }

    bool TraverseCallExpr(const std::shared_ptr<ast::CallExpr>& call) {
        if (!call) return true;
        if (call->callee && !visit(call->callee)) return false;
        for (auto& arg : call->args) {
            if (arg && !visit(arg)) return false;
        }
        return true;
    }

    bool VisitCallExpr(const std::shared_ptr<ast::CallExpr>& call) {
        if (!call) return true;
        std::cout << "CallExpr (args: " << call->args.size() << ")" << std::endl;
        return derived().TraverseCallExpr(call);
    }

    // ------------------------------------------------------------------------
    // PostfixExpr
    // ------------------------------------------------------------------------
    bool WalkUpFromPostfixExpr(const std::shared_ptr<ast::PostfixExpr>& post) {
        if (!derived().VisitPostfixExpr(post)) return false;
        return true;
    }

    bool TraversePostfixExpr(const std::shared_ptr<ast::PostfixExpr>& post) {
        if (!post) return true;
        if (post->expr && !visit(post->expr)) return false;
        if (post->op == ast::opcode::ARRAY && post->array)
            if (!visit(post->array)) return false;
        return true;
    }

    bool VisitPostfixExpr(const std::shared_ptr<ast::PostfixExpr>& post) {
        if (!post) return true;
        static const char* opNames[] = {
            "COMMA", "ASSIGN", "PLUSASSIGN", "MINUSASSIGN", "MULASSIGN", "DIVASSIGN", "MODASSIGN",
            "ANDASSIGN", "ORASSIGN", "XORASSIGN", "LSASSIGN", "RSASSIGN",
            "OROR", "ANDAND", "BITOR", "BITXOR", "BITAND", "LSHIFT", "RSHIFT",
            "EQ", "NEQ", "LT", "GT", "LE", "GE",
            "ADD", "SUB", "MUL", "DIV", "MOD",
            "STAR", "AMP", "PLUSPLUS", "MINUSMINUS", "ARRAY", "NONE"
        };
        std::cout << "PostfixExpr [" << opNames[static_cast<int>(post->op)] << "]" << std::endl;
        return derived().TraversePostfixExpr(post);
    }

    // ------------------------------------------------------------------------
    // CastExpr
    // ------------------------------------------------------------------------
    bool WalkUpFromCastExpr(const std::shared_ptr<ast::CastExpr>& cast) {
        if (!derived().VisitCastExpr(cast)) return false;
        return true;
    }

    bool TraverseCastExpr(const std::shared_ptr<ast::CastExpr>& cast) {
        if (!cast) return true;
        if (cast->type && !visit(cast->type)) return false;
        if (cast->expr && !visit(cast->expr)) return false;
        return true;
    }

    bool VisitCastExpr(const std::shared_ptr<ast::CastExpr>& cast) {
        if (!cast) return true;
        std::cout << "CastExpr" << std::endl;
        return derived().TraverseCastExpr(cast);
    }

    // ========================================================================
    // DeclSpecifier (基类)
    // ========================================================================
    bool WalkUpFromDeclSpecifier(const std::shared_ptr<ast::DeclSpecifier>& ds) {
        if (auto sc = std::dynamic_pointer_cast<ast::StorageClassSpec>(ds))
            return derived().WalkUpFromStorageClassSpec(sc);
        if (auto tq = std::dynamic_pointer_cast<ast::TypeQualifierSpec>(ds))
            return derived().WalkUpFromTypeQualifierSpec(tq);
        if (auto ts = std::dynamic_pointer_cast<ast::TypeSpecifier>(ds))
            return derived().WalkUpFromTypeSpecifier(ts);
        return derived().VisitDeclSpecifier(ds);
    }

    bool VisitDeclSpecifier(const std::shared_ptr<ast::DeclSpecifier>& ds) {
        if (!ds) return true;
        std::cout << "DeclSpecifier" << std::endl;
        return true;
    }

    // ------------------------------------------------------------------------
    // StorageClassSpec
    // ------------------------------------------------------------------------
    bool WalkUpFromStorageClassSpec(const std::shared_ptr<ast::StorageClassSpec>& sc) {
        if (!derived().VisitStorageClassSpec(sc)) return false;
        return true;
    }

    bool VisitStorageClassSpec(const std::shared_ptr<ast::StorageClassSpec>& sc) {
        if (!sc) return true;
        static const char* names[] = {"NONE", "auto", "register", "static", "extern", "typedef"};
        int idx = static_cast<int>(sc->storageClass);
        if (idx >= 0 && idx <= 5)
            std::cout << "StorageClassSpec: " << names[idx] << std::endl;
        return true;
    }

    // ------------------------------------------------------------------------
    // TypeQualifierSpec
    // ------------------------------------------------------------------------
    bool WalkUpFromTypeQualifierSpec(const std::shared_ptr<ast::TypeQualifierSpec>& tq) {
        if (!derived().VisitTypeQualifierSpec(tq)) return false;
        return true;
    }

    bool VisitTypeQualifierSpec(const std::shared_ptr<ast::TypeQualifierSpec>& tq) {
        if (!tq) return true;
        static const char* names[] = {"const", "volatile", "restrict", "atomic", "NONE"};
        int idx = static_cast<int>(tq->qualifier);
        if (idx >= 0 && idx <= 4)
            std::cout << "TypeQualifierSpec: " << names[idx] << std::endl;
        return true;
    }

    // ------------------------------------------------------------------------
    // TypeSpecifier (基类)
    // ------------------------------------------------------------------------
    bool WalkUpFromTypeSpecifier(const std::shared_ptr<ast::TypeSpecifier>& ts) {
        if (auto bt = std::dynamic_pointer_cast<ast::BuiltinTypeSpec>(ts))
            return derived().WalkUpFromBuiltinTypeSpec(bt);
        if (auto st = std::dynamic_pointer_cast<ast::StructTypeSpec>(ts))
            return derived().WalkUpFromStructTypeSpec(st);
        if (auto ut = std::dynamic_pointer_cast<ast::UnionTypeSpec>(ts))
            return derived().WalkUpFromUnionTypeSpec(ut);
        if (auto et = std::dynamic_pointer_cast<ast::EnumTypeSpec>(ts))
            return derived().WalkUpFromEnumTypeSpec(et);
        
        return derived().VisitTypeSpecifier(ts);
    }

    bool VisitTypeSpecifier(const std::shared_ptr<ast::TypeSpecifier>& ts) {
        (void)ts;
        return true;
    }

    bool WalkUpFromBuiltinTypeSpec(const std::shared_ptr<ast::BuiltinTypeSpec>& bts) {
        if (!derived().VisitBuiltinTypeSpec(bts)) return false;
        return true;
    }

    bool VisitBuiltinTypeSpec(const std::shared_ptr<ast::BuiltinTypeSpec>& bts) {
        if (!bts) return true;
        static const char* names[] = {"void", "char", "short", "int", "long", "float", "double", "signed", "unsigned", "bool", "NONE"};
        int idx = static_cast<int>(bts->builtin);
        if (idx >= 0 && idx <= 10)
            std::cout << "BuiltinTypeSpec: " << names[idx] << std::endl;
        return true;
    }

    bool WalkUpFromStructTypeSpec(const std::shared_ptr<ast::StructTypeSpec>& sts) {
        if (!derived().VisitStructTypeSpec(sts)) return false;
        return true;
    }

    bool TraverseStructTypeSpec(const std::shared_ptr<ast::StructTypeSpec>& sts) {
        if (!sts) return true;
        for (auto& f : sts->fields)
            if (f && !visit(f)) return false;
        return true;
    }

    bool VisitStructTypeSpec(const std::shared_ptr<ast::StructTypeSpec>& sts) {
        if (!sts) return true;
        if (!sts->name.empty())
            std::cout << "StructTypeSpec: " << sts->name << std::endl;
        return derived().TraverseStructTypeSpec(sts);
    }

    bool WalkUpFromUnionTypeSpec(const std::shared_ptr<ast::UnionTypeSpec>& uts) {
        if (!derived().VisitUnionTypeSpec(uts)) return false;
        return true;
    }

    bool TraverseUnionTypeSpec(const std::shared_ptr<ast::UnionTypeSpec>& uts) {
        if (!uts) return true;
        for (auto& f : uts->fields)
            if (f && !visit(f)) return false;
        return true;
    }

    bool VisitUnionTypeSpec(const std::shared_ptr<ast::UnionTypeSpec>& uts) {
        if (!uts) return true;
        if (!uts->name.empty())
            std::cout << "UnionTypeSpec: " << uts->name << std::endl;
        return derived().TraverseUnionTypeSpec(uts);
    }

    bool WalkUpFromEnumTypeSpec(const std::shared_ptr<ast::EnumTypeSpec>& ets) {
        if (!derived().VisitEnumTypeSpec(ets)) return false;
        return true;
    }

    bool TraverseEnumTypeSpec(const std::shared_ptr<ast::EnumTypeSpec>& ets) {
        if (!ets) return true;
        for (auto& e : ets->enumerators)
            if (e.value && !visit(e.value)) return false;
        return true;
    }

    bool VisitEnumTypeSpec(const std::shared_ptr<ast::EnumTypeSpec>& ets) {
        if (!ets) return true;
        if (!ets->name.empty())
            std::cout << "EnumTypeSpec: " << ets->name << std::endl;
        return derived().TraverseEnumTypeSpec(ets);
    }

    // ========================================================================
    // DeclSpec
    // ========================================================================
    bool WalkUpFromDeclSpec(const std::shared_ptr<ast::DeclSpec>& ds) {
        if (!derived().VisitDeclSpec(ds)) return false;
        return true;
    }

    bool TraverseDeclSpec(const std::shared_ptr<ast::DeclSpec>& ds) {
        if (!ds) return true;
        for (auto& s : ds->specs)
            if (s && !visit(s)) return false;
        return true;
    }

    bool VisitDeclSpec(const std::shared_ptr<ast::DeclSpec>& ds) {
        if (!ds) return true;
        return derived().TraverseDeclSpec(ds);
    }

    // ========================================================================
    // Declarator
    // ========================================================================
    bool WalkUpFromDeclarator(const std::shared_ptr<ast::Declarator>& decl) {
        if (!derived().VisitDeclarator(decl)) return false;
        return true;
    }

    bool TraverseDeclarator(const std::shared_ptr<ast::Declarator>& decl) {
        if (!decl) return true;
        if (decl->pointer && !visit(decl->pointer)) return false;
        if (decl->direct && !visit(decl->direct)) return false;
        return true;
    }

    bool VisitDeclarator(const std::shared_ptr<ast::Declarator>& decl) {
        if (!decl) return true;
        std::cout << "Declarator (hasPointer: " << (decl->pointer ? "true" : "false") << ")" << std::endl;
        return derived().TraverseDeclarator(decl);
    }

    // ========================================================================
    // Pointer
    // ========================================================================
    bool WalkUpFromPointer(const std::shared_ptr<ast::Pointer>& ptr) {
        if (!derived().VisitPointer(ptr)) return false;
        return true;
    }

    bool TraversePointer(const std::shared_ptr<ast::Pointer>& ptr) {
        if (!ptr) return true;
        for (auto& q : ptr->qualifiers)
            if (q && !visit(q)) return false;
        if (ptr->next && !visit(ptr->next)) return false;
        return true;
    }

    bool VisitPointer(const std::shared_ptr<ast::Pointer>& ptr) {
        if (!ptr) return true;
        std::cout << "Pointer (qualifiers: " << ptr->qualifiers.size() 
                  << ", hasMore: " << (ptr->next ? "true" : "false") << ")" << std::endl;
        return derived().TraversePointer(ptr);
    }

    // ========================================================================
    // DirectDeclarator (基类)
    // ========================================================================
    bool WalkUpFromDirectDeclarator(const std::shared_ptr<ast::DirectDeclarator>& dd) {
        if (auto id = std::dynamic_pointer_cast<ast::DDIdentifier>(dd))
            return derived().WalkUpFromDDIdentifier(id);
        if (auto p = std::dynamic_pointer_cast<ast::DDParen>(dd))
            return derived().WalkUpFromDDParen(p);
        if (auto a = std::dynamic_pointer_cast<ast::DDArray>(dd))
            return derived().WalkUpFromDDArray(a);
        if (auto c = std::dynamic_pointer_cast<ast::DDCall>(dd))
            return derived().WalkUpFromDDCall(c);
        
        return derived().VisitDirectDeclarator(dd);
    }

    bool VisitDirectDeclarator(const std::shared_ptr<ast::DirectDeclarator>& dd) {
        (void)dd;
        return true;
    }

    bool WalkUpFromDDIdentifier(const std::shared_ptr<ast::DDIdentifier>& id) {
        if (!derived().VisitDDIdentifier(id)) return false;
        return true;
    }

    bool VisitDDIdentifier(const std::shared_ptr<ast::DDIdentifier>& id) {
        if (!id) return true;
        std::cout << "DDIdentifier: " << id->name << std::endl;
        return true;
    }

    bool WalkUpFromDDParen(const std::shared_ptr<ast::DDParen>& dp) {
        if (!derived().VisitDDParen(dp)) return false;
        return true;
    }

    bool TraverseDDParen(const std::shared_ptr<ast::DDParen>& dp) {
        if (!dp) return true;
        if (dp->inner && !visit(dp->inner)) return false;
        return true;
    }

    bool VisitDDParen(const std::shared_ptr<ast::DDParen>& dp) {
        if (!dp) return true;
        return derived().TraverseDDParen(dp);
    }

    bool WalkUpFromDDArray(const std::shared_ptr<ast::DDArray>& da) {
        if (!derived().VisitDDArray(da)) return false;
        return true;
    }

    bool TraverseDDArray(const std::shared_ptr<ast::DDArray>& da) {
        if (!da) return true;
        if (da->base && !visit(da->base)) return false;
        if (da->size && !visit(da->size)) return false;
        return true;
    }

    bool VisitDDArray(const std::shared_ptr<ast::DDArray>& da) {
        if (!da) return true;
        std::cout << "DDArray (hasSize: " << (da->size ? "true" : "false") << ")" << std::endl;
        return derived().TraverseDDArray(da);
    }

    bool WalkUpFromDDCall(const std::shared_ptr<ast::DDCall>& dc) {
        if (!derived().VisitDDCall(dc)) return false;
        return true;
    }

    bool TraverseDDCall(const std::shared_ptr<ast::DDCall>& dc) {
        if (!dc) return true;
        if (dc->base && !visit(dc->base)) return false;
        for (auto& p : dc->params)
            if (p && !visit(p)) return false;
        return true;
    }

    bool VisitDDCall(const std::shared_ptr<ast::DDCall>& dc) {
        if (!dc) return true;
        std::cout << "DDCall (params: " << dc->params.size() << ")" << std::endl;
        return derived().TraverseDDCall(dc);
    }

    // ========================================================================
    // AbstractDeclarator
    // ========================================================================
    bool WalkUpFromAbstractDeclarator(const std::shared_ptr<ast::AbstractDeclarator>& ad) {
        if (!derived().VisitAbstractDeclarator(ad)) return false;
        return true;
    }

    bool TraverseAbstractDeclarator(const std::shared_ptr<ast::AbstractDeclarator>& ad) {
        if (!ad) return true;
        if (ad->pointer && !visit(ad->pointer)) return false;
        if (ad->direct && !visit(ad->direct)) return false;
        return true;
    }

    bool VisitAbstractDeclarator(const std::shared_ptr<ast::AbstractDeclarator>& ad) {
        if (!ad) return true;
        return derived().TraverseAbstractDeclarator(ad);
    }

    // ========================================================================
    // DirectAbstractDeclarator (基类)
    // ========================================================================
    bool WalkUpFromDirectAbstractDeclarator(const std::shared_ptr<ast::DirectAbstractDeclarator>& dad) {
        if (auto p = std::dynamic_pointer_cast<ast::DAParen>(dad))
            return derived().WalkUpFromDAParen(p);
        if (auto a = std::dynamic_pointer_cast<ast::DAArray>(dad))
            return derived().WalkUpFromDAArray(a);
        if (auto c = std::dynamic_pointer_cast<ast::DACall>(dad))
            return derived().WalkUpFromDACall(c);
        
        return derived().VisitDirectAbstractDeclarator(dad);
    }

    bool VisitDirectAbstractDeclarator(const std::shared_ptr<ast::DirectAbstractDeclarator>& dad) {
        (void)dad;
        return true;
    }

    bool WalkUpFromDAParen(const std::shared_ptr<ast::DAParen>& dap) {
        if (!derived().VisitDAParen(dap)) return false;
        return true;
    }

    bool TraverseDAParen(const std::shared_ptr<ast::DAParen>& dap) {
        if (!dap) return true;
        if (dap->inner && !visit(dap->inner)) return false;
        return true;
    }

    bool VisitDAParen(const std::shared_ptr<ast::DAParen>& dap) {
        if (!dap) return true;
        return derived().TraverseDAParen(dap);
    }

    bool WalkUpFromDAArray(const std::shared_ptr<ast::DAArray>& daa) {
        if (!derived().VisitDAArray(daa)) return false;
        return true;
    }

    bool TraverseDAArray(const std::shared_ptr<ast::DAArray>& daa) {
        if (!daa) return true;
        if (daa->base && !visit(daa->base)) return false;
        if (daa->size && !visit(daa->size)) return false;
        return true;
    }

    bool VisitDAArray(const std::shared_ptr<ast::DAArray>& daa) {
        if (!daa) return true;
        return derived().TraverseDAArray(daa);
    }

    bool WalkUpFromDACall(const std::shared_ptr<ast::DACall>& dac) {
        if (!derived().VisitDACall(dac)) return false;
        return true;
    }

    bool TraverseDACall(const std::shared_ptr<ast::DACall>& dac) {
        if (!dac) return true;
        if (dac->base && !visit(dac->base)) return false;
        for (auto& p : dac->params)
            if (p && !visit(p)) return false;
        return true;
    }

    bool VisitDACall(const std::shared_ptr<ast::DACall>& dac) {
        if (!dac) return true;
        return derived().TraverseDACall(dac);
    }

    // ========================================================================
    // TypeName
    // ========================================================================
    bool WalkUpFromTypeName(const std::shared_ptr<ast::TypeName>& tn) {
        if (!derived().VisitTypeName(tn)) return false;
        return true;
    }

    bool TraverseTypeName(const std::shared_ptr<ast::TypeName>& tn) {
        if (!tn) return true;
        if (tn->specs && !visit(tn->specs)) return false;
        if (tn->abstractDecl && !visit(tn->abstractDecl)) return false;
        return true;
    }

    bool VisitTypeName(const std::shared_ptr<ast::TypeName>& tn) {
        if (!tn) return true;
        std::cout << "TypeName (hasAbstractDeclarator: " << (tn->abstractDecl ? "true" : "false") << ")" << std::endl;
        return derived().TraverseTypeName(tn);
    }

    // ========================================================================
    // ParameterDecl
    // ========================================================================
    bool WalkUpFromParameterDecl(const std::shared_ptr<ast::ParameterDecl>& pd) {
        if (!derived().VisitParameterDecl(pd)) return false;
        return true;
    }

    bool TraverseParameterDecl(const std::shared_ptr<ast::ParameterDecl>& pd) {
        if (!pd) return true;
        if (pd->specs && !visit(pd->specs)) return false;
        if (pd->declarator && !visit(pd->declarator)) return false;
        return true;
    }

    bool VisitParameterDecl(const std::shared_ptr<ast::ParameterDecl>& pd) {
        if (!pd) return true;
        std::cout << "ParameterDecl (hasDeclarator: " << (pd->declarator ? "true" : "false") << ")" << std::endl;
        return derived().TraverseParameterDecl(pd);
    }

    // ========================================================================
    // Initializer (基类)
    // ========================================================================
    bool WalkUpFromInitializer(const std::shared_ptr<ast::Initializer>& init) {
        if (auto ei = std::dynamic_pointer_cast<ast::ExprInitializer>(init))
            return derived().WalkUpFromExprInitializer(ei);
        if (auto il = std::dynamic_pointer_cast<ast::InitList>(init))
            return derived().WalkUpFromInitList(il);
        
        return derived().VisitInitializer(init);
    }

    bool VisitInitializer(const std::shared_ptr<ast::Initializer>& init) {
        (void)init;
        return true;
    }

    bool WalkUpFromExprInitializer(const std::shared_ptr<ast::ExprInitializer>& ei) {
        if (!derived().VisitExprInitializer(ei)) return false;
        return true;
    }

    bool TraverseExprInitializer(const std::shared_ptr<ast::ExprInitializer>& ei) {
        if (!ei) return true;
        if (ei->expr && !visit(ei->expr)) return false;
        return true;
    }

    bool VisitExprInitializer(const std::shared_ptr<ast::ExprInitializer>& ei) {
        if (!ei) return true;
        std::cout << "ExprInitializer" << std::endl;
        return derived().TraverseExprInitializer(ei);
    }

    bool WalkUpFromInitList(const std::shared_ptr<ast::InitList>& il) {
        if (!derived().VisitInitList(il)) return false;
        return true;
    }

    bool TraverseInitList(const std::shared_ptr<ast::InitList>& il) {
        if (!il) return true;
        for (auto& e : il->elements)
            if (e && !visit(e)) return false;
        return true;
    }

    bool VisitInitList(const std::shared_ptr<ast::InitList>& il) {
        if (!il) return true;
        std::cout << "InitList (elements: " << il->elements.size() << ")" << std::endl;
        return derived().TraverseInitList(il);
    }

    // ========================================================================
    // InitDeclarator
    // ========================================================================
    bool WalkUpFromInitDeclarator(const std::shared_ptr<ast::InitDeclarator>& id) {
        if (!derived().VisitInitDeclarator(id)) return false;
        return true;
    }

    bool TraverseInitDeclarator(const std::shared_ptr<ast::InitDeclarator>& id) {
        if (!id) return true;
        if (id->declarator && !visit(id->declarator)) return false;
        if (id->initializer && !visit(id->initializer)) return false;
        return true;
    }

    bool VisitInitDeclarator(const std::shared_ptr<ast::InitDeclarator>& id) {
        if (!id) return true;
        std::cout << "InitDeclarator (hasInit: " << (id->initializer ? "true" : "false") << ")" << std::endl;
        return derived().TraverseInitDeclarator(id);
    }

    // ========================================================================
    // BlockItem (基类)
    // ========================================================================
    bool WalkUpFromBlockItem(const std::shared_ptr<ast::BlockItem>& bi) {
        if (auto di = std::dynamic_pointer_cast<ast::DeclItem>(bi))
            return derived().WalkUpFromDeclItem(di);
        if (auto si = std::dynamic_pointer_cast<ast::StmtItem>(bi))
            return derived().WalkUpFromStmtItem(si);
        
        return derived().VisitBlockItem(bi);
    }

    bool VisitBlockItem(const std::shared_ptr<ast::BlockItem>& bi) {
        (void)bi;
        return true;
    }

    bool WalkUpFromDeclItem(const std::shared_ptr<ast::DeclItem>& di) {
        if (!derived().VisitDeclItem(di)) return false;
        return true;
    }

    bool TraverseDeclItem(const std::shared_ptr<ast::DeclItem>& di) {
        if (!di) return true;
        if (di->decl && !visit(di->decl)) return false;
        return true;
    }

    bool VisitDeclItem(const std::shared_ptr<ast::DeclItem>& di) {
        if (!di) return true;
        return derived().TraverseDeclItem(di);
    }

    bool WalkUpFromStmtItem(const std::shared_ptr<ast::StmtItem>& si) {
        if (!derived().VisitStmtItem(si)) return false;
        return true;
    }

    bool TraverseStmtItem(const std::shared_ptr<ast::StmtItem>& si) {
        if (!si) return true;
        if (si->stmt && !visit(si->stmt)) return false;
        return true;
    }

    bool VisitStmtItem(const std::shared_ptr<ast::StmtItem>& si) {
        if (!si) return true;
        return derived().TraverseStmtItem(si);
    }

    // ========================================================================
    // StructDecl / StructSpec / UnionSpec / EnumSpec / Enumerator
    // ========================================================================
    bool WalkUpFromStructDecl(const std::shared_ptr<ast::StructDecl>& sd) {
        if (!derived().VisitStructDecl(sd)) return false;
        return true;
    }

    bool TraverseStructDecl(const std::shared_ptr<ast::StructDecl>& sd) {
        if (!sd) return true;
        if (sd->specs && !visit(sd->specs)) return false;
        for (auto& d : sd->declarators)
            if (d && !visit(d)) return false;
        return true;
    }

    bool VisitStructDecl(const std::shared_ptr<ast::StructDecl>& sd) {
        if (!sd) return true;
        return derived().TraverseStructDecl(sd);
    }

    bool WalkUpFromStructSpec(const std::shared_ptr<ast::StructSpec>& ss) {
        if (!derived().VisitStructSpec(ss)) return false;
        return true;
    }

    bool TraverseStructSpec(const std::shared_ptr<ast::StructSpec>& ss) {
        if (!ss) return true;
        for (auto& m : ss->members)
            if (m && !visit(m)) return false;
        return true;
    }

    bool VisitStructSpec(const std::shared_ptr<ast::StructSpec>& ss) {
        if (!ss) return true;
        if (!ss->name.empty())
            std::cout << "StructSpec: " << ss->name << std::endl;
        return derived().TraverseStructSpec(ss);
    }

    bool WalkUpFromUnionSpec(const std::shared_ptr<ast::UnionSpec>& us) {
        if (!derived().VisitUnionSpec(us)) return false;
        return true;
    }

    bool TraverseUnionSpec(const std::shared_ptr<ast::UnionSpec>& us) {
        if (!us) return true;
        for (auto& m : us->members)
            if (m && !visit(m)) return false;
        return true;
    }

    bool VisitUnionSpec(const std::shared_ptr<ast::UnionSpec>& us) {
        if (!us) return true;
        if (!us->name.empty())
            std::cout << "UnionSpec: " << us->name << std::endl;
        return derived().TraverseUnionSpec(us);
    }

    bool WalkUpFromEnumerator(const std::shared_ptr<ast::Enumerator>& e) {
        if (!derived().VisitEnumerator(e)) return false;
        return true;
    }

    bool TraverseEnumerator(const std::shared_ptr<ast::Enumerator>& e) {
        if (!e) return true;
        if (e->value && !visit(e->value)) return false;
        return true;
    }

    bool VisitEnumerator(const std::shared_ptr<ast::Enumerator>& e) {
        if (!e) return true;
        std::cout << "Enumerator: " << e->name << std::endl;
        return derived().TraverseEnumerator(e);
    }

    bool WalkUpFromEnumSpec(const std::shared_ptr<ast::EnumSpec>& es) {
        if (!derived().VisitEnumSpec(es)) return false;
        return true;
    }

    bool TraverseEnumSpec(const std::shared_ptr<ast::EnumSpec>& es) {
        if (!es) return true;
        for (auto& en : es->enumerators)
            if (en && !visit(en)) return false;
        return true;
    }

    bool VisitEnumSpec(const std::shared_ptr<ast::EnumSpec>& es) {
        if (!es) return true;
        if (!es->name.empty())
            std::cout << "EnumSpec: " << es->name << std::endl;
        return derived().TraverseEnumSpec(es);
    }
};

// ============================================================================
// 便捷别名，保持向后兼容
// ============================================================================
template <class Derived>
using ASTVisitor = RecursiveASTVisitor<Derived>;

} // namespace rcc::front