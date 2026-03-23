#pragma once

#include "AST.h"
#include "ASTVisitor.h"

#include <memory>
#include <ostream>

namespace rcc::front {

class ASTPrinter : public ASTVisitor<ASTPrinter> {
public:
    explicit ASTPrinter(std::ostream& out);
    bool print(const std::shared_ptr<ast::TranslationUnit>& tu);

private:
    std::ostream& out_;
};

} // namespace rcc::front
