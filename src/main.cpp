// Author: KnOFCA

#include <any>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>

#include "antlr4-runtime.h"
#include "./ANTLR/TLexer.h"
#include "./ANTLR/TParser.h"
#include "./backend/RISCV64Backend.h"
#include "./front/AST.h"
#include "./front/ASTBuilder.h"
#include "./front/ASTPrinter.h"
#include "./ir/IRBuilder.h"

using namespace antlrcpptest;
using namespace antlr4;

namespace {

enum class EmitKind {
  AST,
  IR,
  ASM,
};

struct CommandLineOptions {
  std::string input_path;
  std::optional<std::string> output_path;
  EmitKind emit_kind{EmitKind::IR};
};

void print_usage(const char *argv0) {
  std::cerr << "usage: " << argv0 << " <input.c> [-emit-ast | -emit-ir | -S] [-o <output>]\n";
}

std::optional<CommandLineOptions> parse_args(int argc, const char **argv) {
  CommandLineOptions options;

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "-emit-ir") {
      options.emit_kind = EmitKind::IR;
      continue;
    }
    if (arg == "-emit-ast") {
      options.emit_kind = EmitKind::AST;
      continue;
    }
    if (arg == "-S") {
      options.emit_kind = EmitKind::ASM;
      continue;
    }
    if (arg == "-o") {
      if (i + 1 >= argc) {
        std::cerr << "missing output path after -o\n";
        return std::nullopt;
      }
      options.output_path = argv[++i];
      continue;
    }
    if (!arg.empty() && arg[0] == '-') {
      std::cerr << "unknown option: " << arg << '\n';
      return std::nullopt;
    }
    if (!options.input_path.empty()) {
      std::cerr << "multiple input files are not supported\n";
      return std::nullopt;
    }
    options.input_path = arg;
  }

  if (options.input_path.empty()) {
    std::cerr << "missing input file\n";
    return std::nullopt;
  }
  return options;
}

} // namespace

int main(int argc, const char **argv) {
  auto options = parse_args(argc, argv);
  if (!options.has_value()) {
    print_usage(argv[0]);
    return 1;
  }

  std::ifstream file(options->input_path);
  if (!file.is_open()) {
    std::cerr << "can not open file: " << options->input_path << std::endl;
    return 1;
  }

  ANTLRInputStream input(file);
  TLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  TParser parser(&tokens);
  tree::ParseTree *tree = parser.translationUnit();
  if (parser.getNumberOfSyntaxErrors() != 0) {
    std::cerr << "parse failed with " << parser.getNumberOfSyntaxErrors()
              << " syntax error(s)" << std::endl;
    return 1;
  }

  rcc::front::ASTBuilder builder;
  auto tu = std::any_cast<std::shared_ptr<ast::TranslationUnit>>(builder.visit(tree));

  if (options->emit_kind == EmitKind::AST) {
    if (options->output_path.has_value()) {
      std::ofstream output_file(*options->output_path);
      if (!output_file.is_open()) {
        std::cerr << "can not open output file: " << *options->output_path << std::endl;
        return 1;
      }
      rcc::front::ASTPrinter printer(output_file);
      if (!printer.print(tu)) {
        std::cerr << "failed to print AST" << std::endl;
        return 1;
      }
      if (!output_file.good()) {
        std::cerr << "failed to write AST output" << std::endl;
        return 1;
      }
    } else {
      rcc::front::ASTPrinter printer(std::cout);
      if (!printer.print(tu)) {
        std::cerr << "failed to print AST" << std::endl;
        return 1;
      }
      if (!std::cout.good()) {
        std::cerr << "failed to write AST output" << std::endl;
        return 1;
      }
    }
    return 0;
  }

  rcc::ir::IRBuilder irbuilder;
  auto ir_result = irbuilder.build_from_AST(tu);
  if (ir_result != rcc::ir::ErrorCode::SUCCESS) {
    std::cerr << "failed to build IR" << std::endl;
    return 1;
  }

  auto program = irbuilder.get_program();
  if (!program) {
    std::cerr << "IR program is empty" << std::endl;
    return 1;
  }

  if (options->emit_kind == EmitKind::IR) {
    auto dump_result = options->output_path.has_value()
                           ? irbuilder.dump_to_file(*options->output_path)
                           : irbuilder.dump_to_stdout();
    if (dump_result != rcc::ir::ErrorCode::SUCCESS) {
      std::cerr << "failed to write IR output" << std::endl;
      return 1;
    }
    return 0;
  }

  std::ostringstream asm_buffer;
  auto backend_result = rcc::backend::emit_riscv64(*program, asm_buffer);
  if (!backend_result.ok()) {
    std::cerr << "backend error: " << backend_result.message << std::endl;
    return 1;
  }

  if (options->output_path.has_value()) {
    std::ofstream output_file(*options->output_path);
    if (!output_file.is_open()) {
      std::cerr << "can not open output file: " << *options->output_path << std::endl;
      return 1;
    }
    output_file << asm_buffer.str();
    if (!output_file.good()) {
      std::cerr << "failed to write assembly output" << std::endl;
      return 1;
    }
  } else {
    std::cout << asm_buffer.str();
    if (!std::cout.good()) {
      std::cerr << "failed to write assembly output" << std::endl;
      return 1;
    }
  }

  return 0;
}