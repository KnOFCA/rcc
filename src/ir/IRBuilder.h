#pragma once

#include "koopa.h"
#include "front/AST.h"
#include "front/ASTVisitor.h"

#include <filesystem>
#include <span>


namespace rcc::ir {

class IRBuilder : public front::ASTVisitor<IRBuilder> {
  public:
    std::shared_ptr<Program> program;
    
    ///
    /// Generate Koopa IR program from AST.
    /// Updates the `program` if no errors occurred.
    ///
    /// Returns the error code.
    ///
    ErrorCode parse_from_AST(std::shared_ptr<ast::TranslationUnit> node);

    ///
    /// Parses text-form Koopa IR program from the given file.
    /// Updates the `program` if no errors occurred.
    ///
    /// Returns the error code.
    ///
    ErrorCode parse_from_file(const std::filesystem::path& path);

    ///
    /// Parses text-form Koopa IR program from the given string.
    /// Updates the `program` if no errors occurred.
    ///
    /// Returns the error code.
    ///
    ErrorCode parse_from_string(std::string_view str);

    ///
    /// Parses text-form Koopa IR program from the standard input.
    /// Updates the `program` if no errors occurred.
    ///
    /// Returns the error code.
    ///
    ErrorCode parse_from_stdin();

    ///
    /// Parses text-form Koopa IR program from the given
    /// file descriptor (UNIX) or handle (Windows).
    /// Updates the `program` if no errors occurred.
    ///
    /// Returns the error code.
    ///
    ErrorCode parse_from_raw(File file);

    ///
    /// Deletes the given program.
    ///
    /// All programs returned by Koopa IR library functions
    /// should be deleted manually.
    ///
    void delete_program();

    ///
    /// Generates text-form Koopa IR program to the given file.
    ///
    /// Returns the error code.
    ///
    ErrorCode dump_to_file(const std::filesystem::path& path);

    ///
    /// Generates a null-terminated string of text-form Koopa IR program
    /// to the given buffer. If the given buffer is null, updates the `len`
    /// to the length of the generated string (with out the null-terminator).
    ///
    /// Returns the error code.
    ///
    ErrorCode dump_to_string(std::span<char> buffer, std::size_t* len);

    ///
    /// Generates text-form Koopa IR program to the standard output.
    ///
    /// Returns the error code.
    ///
    ErrorCode dump_to_stdout();

    ///
    /// Generates text-form Koopa IR program to the given
    /// file descriptor (UNIX) or handle (Windows).
    ///
    /// Returns the error code.
    ///
    ErrorCode dump_to_raw(File file);

    ///
    /// Generates LLVM IR to the given file.
    ///
    /// Returns the error code.
    ///
    ErrorCode dump_llvm_to_file(const std::filesystem::path& path);

    ///
    /// Generates a null-terminated string of LLVM IR to the given buffer.
    /// If the given buffer is null, updates the `len` to the length of
    /// the generated string (with out the null-terminator).
    ///
    /// Returns the error code.
    ///
    ErrorCode dump_llvm_to_string(std::span<char> buffer, std::size_t* len);

    ///
    /// Generates LLVM IR to the standard output.
    ///
    /// Returns the error code.
    ///
    ErrorCode dump_llvm_to_stdout();

    ///
    /// Generates LLVM IR to the given
    /// file descriptor (UNIX) or handle (Windows).
    ///
    /// Returns the error code.
    ///
    ErrorCode dump_llvm_to_raw(File file);

    ///
    /// Generates the given raw program to the Koopa IR program.
    /// Updates the `program` if no errors occurred.
    ///
    /// Returns the error code.
    ///
    ErrorCode generate_to_koopa(const Program* raw, Program* program);

    ///
    /// visit method.
    ///
    void visitTranslationUnit(const std::shared_ptr<ast::TranslationUnit> &tu) override;

    void visitDeclaration(const std::shared_ptr<ast::Declaration> &decl) override;
    void visitFunctionDef(const std::shared_ptr<ast::FunctionDef> &fdef) override;

    void visitStmt(const std::shared_ptr<ast::Stmt> &stmt) override;
    void visitCompoundStmt(const std::shared_ptr<ast::CompoundStmt> &comp) override;

    void visitExpr(const std::shared_ptr<ast::Expr> &expr) override;
    void visitIdExpr(const std::shared_ptr<ast::IdExpr> &id) override;
    void visitLiteralExpr(const std::shared_ptr<ast::LiteralExpr> &lit) override;
    void visitUnaryExpr(const std::shared_ptr<ast::UnaryExpr> &un) override;
    void visitBinaryExpr(const std::shared_ptr<ast::BinaryExpr> &bin) override;
    void visitConditionalExpr(const std::shared_ptr<ast::ConditionalExpr> &ce) override;
    void visitCallExpr(const std::shared_ptr<ast::CallExpr> &call) override;
    void visitPostfixExpr(const std::shared_ptr<ast::PostfixExpr> &post) override;

    void visitDeclarator(const std::shared_ptr<ast::Declarator> &decl) override;
    void visitTypeName(const std::shared_ptr<ast::TypeName> &tn) override;

};

}