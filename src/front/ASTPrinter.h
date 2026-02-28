#pragma once

#include "AST.h"
#include "ASTVisitor.h"

#include <memory>

#include <iostream>

namespace rcc::front {

class ASTPrinter : public ASTVisitor<ASTPrinter> {
  public:
    //using ASTVisitor<ASTPrinter>::visitImpl;
    ///
    /// visit method.
    ///

    // void visitImpl(const std::shared_ptr<ast::Declarator> &decl) { 
    //     if (decl->pointer) visit(decl->pointer);
    //     if (decl->direct) visit(decl->direct);
    // }

    // void visitImpl(const std::shared_ptr<ast::IdExpr> &id) { std::cout << id->name; }
};
}