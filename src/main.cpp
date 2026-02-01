/* Copyright (c) 2012-2017 The ANTLR Project. All rights reserved.
 * Use of this file is governed by the BSD 3-clause license that
 * can be found in the LICENSE.txt file in the project root.
 */

//
//  main.cpp
//  antlr4-cpp-demo
//
//  Created by Mike Lischke on 13.03.16.
//

#include <any>
#include <iostream>
#include <memory>

#include "antlr4-runtime.h"
#include "./ANTLR/TLexer.h"
#include "./ANTLR/TParser.h"
#include "./front/ASTBuilder.h"
#include "./front/AST.h"

using namespace antlrcpptest;
using namespace antlr4;

int main(int , const char **) {
  ANTLRInputStream input("int a;");
  TLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();
  for (auto token : tokens.getTokens()) {
    std::cout << token->toString() << std::endl;
  }

  TParser parser(&tokens);
  tree::ParseTree* tree = parser.translationUnit();

  rcc::ASTBuilder builder;
  auto tu = std::any_cast<std::shared_ptr<ast::TranslationUnit>>(builder.visit(tree));
  auto extDecl = tu->externalDecls[0];

  if(std::dynamic_pointer_cast<std::shared_ptr<ast::FunctionDef>>(extDecl))
    std::cout << "funcDef";
  else if(std::dynamic_pointer_cast<std::shared_ptr<ast::Declaration>>(extDecl))
    std::cout << "declaration";
  else std::cout << "no match";

  std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

  return 0;
}
