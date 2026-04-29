// Author: KnOFCA

#include "ASTPrinter.h"

#include <iostream>

namespace rcc::front {

ASTPrinter::ASTPrinter(std::ostream& out) : out_(out) {}

bool ASTPrinter::print(const std::shared_ptr<ast::TranslationUnit>& tu) {
    auto* previous = std::cout.rdbuf(out_.rdbuf());
    const bool ok = visit(tu);
    std::cout.rdbuf(previous);
    return ok;
}

} // namespace rcc::front