#include <any>
#include <iostream>
#include <memory>
#include <fstream>

#include "antlr4-runtime.h"
#include "./ANTLR/TLexer.h"
#include "./ANTLR/TParser.h"
#include "./front/ASTBuilder.h"
#include "./front/AST.h"

using namespace antlrcpptest;
using namespace antlr4;

int main(int argc, const char **argv) {
  std::ifstream file;
  std::istream *inputStream = &std::cin; // default to stdin
  
  if (argc > 1) {
      file.open(argv[1]);
      if (!file.is_open()) {
          std::cerr << "can not open file: " << argv[1] << std::endl;
          return 1;
      }
      inputStream = &file;
  }
  
  antlr4::ANTLRInputStream input(*inputStream);
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
  std::cout << tree::Trees::toStringTree(tree, &parser) << std::endl;
  for (auto child : tree->children) {
  if (auto extDecl = dynamic_cast<TParser::ExternalDeclarationContext*>(child)) {
      if (auto funcDef = extDecl->functionDefinition()) {
        std::cout << "Function Definition subtree:" << std::endl;
        std::cout << antlr4::tree::Trees::toStringTree(funcDef, &parser) << std::endl;
      } else if (auto decl = extDecl->declaration()) {
        std::cout << "Declaration subtree:" << std::endl;
        std::cout << antlr4::tree::Trees::toStringTree(decl, &parser) << std::endl;
      }
    }
  }

  if (file.is_open()) {
      file.close();
  }

  return 0;
}
