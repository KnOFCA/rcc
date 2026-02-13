#pragma once

#include "front/AST.h"
#include "front/ASTVisitor.h"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <optional>
#include <span>
#include <filesystem>
#include <cstdio>
#include <string_view>
#include <variant>

namespace rcc::ir {

///
/// Error code of some Koopa functions.
///
enum class ErrorCode : int {
    /// No errors occurred.
    SUCCESS = 0,
    /// UTF-8 string conversion error.
    INVALID_UTF8_STRING,
    /// File operation error.
    INVALID_FILE,
    /// Koopa IR program parsing error.
    INVALID_PROGRAM,
    /// IO operation error.
    IO_ERROR,
    /// Byte array to C string conversion error.
    NULL_BYTE_ERROR,
    /// Insufficient buffer length.
    INSUFFICIENT_BUFFER_LENGTH,
    /// Mismatch of item kind in raw slice.
    SliceItemKind_MISMATCH,
    /// Passing null pointers to `libkoopa`.
    NULL_POINTER_ERROR,
    /// Mismatch of type.
    TYPE_MISMATCH,
    /// Mismatch of Function parameter number.
    FUNC_PARAM_NUM_MISMATCH
};

///
/// Raw file (file descriptor).
///
using File = std::FILE*;

///
/// Kind of raw slice item.
///
enum class SliceItemKind : uint32_t {
    /// Unknown.
    UNKNOWN = 0,
    /// Type.
    TYPE,
    /// Function.
    FUNCTION,
    /// Basic block.
    BASIC_BLOCK,
    /// Value.
    VALUE
};

///
/// forward decl.
///
struct TypeKind;
struct FunctionData;
struct BasicBlockData;
struct ValueData;

///
/// A raw slice that can store any kind of items.
///
struct Slice {
    /// Valid items
    using ItemsPtr = std::variant<std::shared_ptr<TypeKind>,
        std::shared_ptr<FunctionData>,
        std::shared_ptr<BasicBlockData>,
        std::shared_ptr<ValueData>
        >;
    /// Buffer of slice items (owning container preferred).
    std::vector<ItemsPtr> buffer;
    /// Kind of slice items.
    SliceItemKind kind{SliceItemKind::UNKNOWN};
    /// Convenience: length of slice.
    std::size_t len() const noexcept { return buffer.size(); }
};

///
/// Tag of raw Koopa type.
///
enum class TypeTag : uint32_t {
    /// 8-bit integer.
    INT8,
    /// 16-bit integer.
    INT16,
    /// 32-bit integer.
    INT32,
    /// 64-bit integer.
    INT64,
    /// 32-bit float.
    FLOAT,
    /// 64-bit float.
    DOUBLE,
    /// Unit (void).
    UNIT,
    /// Array (with base type and length).
    ARRAY,
    /// Pointer (with base type).
    POINTER,
    /// Function (with parameter types and return type).
    FUNCTION
};

///
/// Kind of raw Koopa type.
///
struct TypeKind {
    TypeTag tag;
    union {
        struct {
            std::shared_ptr<TypeKind> base;
            std::size_t len;
        } array;
        struct {
            std::shared_ptr<TypeKind> base;
        } pointer;
        struct {
            Slice params;
            std::shared_ptr<TypeKind> ret;
        } function;
    } data;
};

///
/// A raw Koopa type.
///
using Type = std::shared_ptr<TypeKind>;

///
/// A raw Koopa program.
///
struct Program {
    /// Global values (global allocations only).
    Slice values;
    /// Function definitions.
    Slice funcs;
};

///
/// Data of raw Koopa function.
///
struct FunctionData {
    /// Type of function.
    Type ty;
    /// Name of function.
    std::optional<std::string> name;
    /// Parameters.
    Slice params;
    /// Basic blocks, empty if is a Function declaration.
    Slice bbs;
};

///
/// A raw Koopa function.
///
using Function = std::shared_ptr<FunctionData>;

///
/// Data of raw Koopa basic block.
///
struct BasicBlockData {
    /// Name of basic block, null if no name.
    std::optional<std::string> name;
    /// Parameters.
    Slice params;
    /// Values that this basic block is used by.
    Slice used_by;
    /// Instructions in this basic block.
    Slice insts;
};

///
/// A raw Koopa basic block.
///
using BasicBlock = std::shared_ptr<BasicBlockData>;

///
/// A raw Koopa value.
///
using Value = std::shared_ptr<ValueData>;

///
/// Raw Integer8 constant.
///
struct Integer8 {
    /// Value of integer.
    int8_t value;
};

///
/// Raw Integer16 constant.
///
struct Integer16 {
    /// Value of integer.
    int16_t value;
};

///
/// Raw Integer32 constant.
///
struct Integer32 {
    /// Value of integer.
    int32_t value;
};

///
/// Raw Integer64 constant.
///
struct Integer64 {
    /// Value of integer.
    int64_t value;
};

///
/// Raw Float constant.
///
struct Float {
    /// Value of integer.
    float_t value;
};

///
/// Raw Double constant.
///
struct Double {
    /// Value of integer.
    double_t value;
};

///
/// Raw number constant.
///
struct Number {
    using number = std::variant<Integer8, Integer16, Integer32, Integer64, Float, Double>;

    number num;
};

///
/// Raw aggregate constant.
///
struct Aggregate {
    /// Elements.
    Slice elems;
};

///
/// Raw Function argument reference.
///
struct FuncArgRef {
    /// Index.
    std::size_t index;
};

///
/// Raw basic block argument reference.
///
struct BlockArgRef {
    /// Index.
    std::size_t index;
};

///
/// Raw global memory allocation.
///
struct GlobalAlloc {
    /// Initializer.
    Value init;
};

///
/// Raw memory load.
///
struct Load {
    /// Source.
    Value src;
};

///
/// Raw memory store.
///
struct Store {
    /// Value.
    Value value;
    /// Destination.
    Value dest;
};

///
/// Raw pointer calculation.
///
struct GetPtr {
    /// Source.
    Value src;
    /// Index.
    Value index;
};

///
/// Raw element pointer calculation.
///
struct GetElemPtr {
    /// Source.
    Value src;
    /// Index.
    Value index;
};

///
/// Raw binary operator.
///
enum class BinaryOp : uint32_t {
    /// Not equal to.
    NOT_EQ,
    /// Equal to.
    EQ,
    /// Greater than.
    GT,
    /// Less than.
    LT,
    /// Greater than or equal to.
    GE,
    /// Less than or equal to.
    LE,
    /// Addition.
    ADD,
    /// Subtraction.
    SUB,
    /// Multiplication.
    MUL,
    /// Division.
    DIV,
    /// Modulo.
    MOD,
    /// Bitwise AND.
    AND,
    /// Bitwise OR.
    OR,
    /// Bitwise XOR.
    XOR,
    /// Shift left logical.
    SHL,
    /// Shift right logical.
    SHR,
    /// Shift right arithmetic.
    SAR
};

///
/// Raw binary operation.
///
struct Binary {
    /// Operator.
    BinaryOp op;
    /// Left-hand side value.
    Value lhs;
    /// Right-hand side value.
    Value rhs;
};

///
/// Raw conditional branch.
///
struct Branch {
    /// Condition.
    Value cond;
    /// Target if condition is `true`.
    BasicBlock true_bb;
    /// Target if condition is `false`.
    BasicBlock false_bb;
    /// Arguments of `true` target..
    Slice true_args;
    /// Arguments of `false` target..
    Slice false_args;
};

///
/// Raw unconditional jump.
///
struct Jump {
    /// Target.
    BasicBlock target;
    /// Arguments of target..
    Slice args;
};

///
/// Raw Function call.
///
struct Call {
    /// Callee.
    Function callee;
    /// Arguments.
    Slice args;
};

///
/// Raw Function return.
///
struct FuncReturn {
    /// Return value, null if no return value.
    Value value;
};

///
/// Tag of raw Koopa value.
///
enum class ValueTag : uint32_t {
    /// Number constant.
    NUMBER,
    /// Zero initializer.
    ZERO_INIT,
    /// Undefined value.
    UNDEF,
    /// Aggregate constant.
    AGGREGATE,
    /// Function argument reference.
    FUNC_ARG_REF,
    /// Basic block argument reference.
    BLOCK_ARG_REF,
    /// Local memory allocation.
    ALLOC,
    /// Global memory allocation.
    GLOBAL_ALLOC,
    /// Memory load.
    LOAD,
    /// Memory store.
    STORE,
    /// Pointer calculation.
    GET_PTR,
    /// Element pointer calculation.
    GET_ELEM_PTR,
    /// Binary operation.
    BINARY,
    /// Conditional branch.
    BRANCH,
    /// Unconditional jump.
    JUMP,
    /// Function call.
    CALL,
    /// Function return.
    RETURN
};

///
/// Kind of raw Koopa value.
///
struct ValueKind {
    ValueTag tag;
    using Data = std::variant<Number,
        Aggregate ,
        FuncArgRef,
        BlockArgRef,
        GlobalAlloc,
        Load,
        Store,
        GetPtr,
        GetElemPtr,
        Binary,
        Branch,
        Jump,
        Call,
        FuncReturn
    >;
    Data data;
};

struct ValueData {
    /// Type of value.
    Type ty;
    /// Name of value, null if no name.
    std::optional<std::string> name;
    /// Values that this Value is used by.
    Slice used_by;
    /// Kind of value.
    ValueKind kind;
};

class IRBuilder : front::ASTVisitor {
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

}  // namespace rcc::ir
