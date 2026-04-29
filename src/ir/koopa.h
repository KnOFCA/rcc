// Author: KnOFCA

#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <optional>
#include <cstdio>
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

    struct array {
        std::shared_ptr<TypeKind> base;
        std::size_t len;
    };
    struct pointer {
        std::shared_ptr<TypeKind> base;
    };
    struct function {
        Slice params;
        std::shared_ptr<TypeKind> ret;
        bool is_variadic{false};
    };

    using Data = std::variant<std::shared_ptr<array>, 
        std::shared_ptr<pointer>, 
        std::shared_ptr<function>>;
    Data data;
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
    /// for SSA only
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
/// for SSA only
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
/// Raw cast operator.
///
enum class CastOp : uint32_t {
    /// Signed integer to floating point.
    SITOFP,
    /// Floating point to signed integer.
    FPTOSI,
    /// Integer scalar conversion with storage-width semantics.
    INTCAST,
    /// Floating point widening conversion.
    FPEXT,
    /// Floating point narrowing conversion.
    FPTRUNC
};

///
/// Raw cast operation.
///
struct Cast {
    /// Operator.
    CastOp op;
    /// Source value.
    Value value;
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
    /// for SSA only
    Slice true_args;
    /// Arguments of `false` target..
    /// for SSA only
    Slice false_args;
};

///
/// Raw unconditional jump.
///
struct Jump {
    /// Target.
    BasicBlock target;
    /// Arguments of target..
    /// for SSA only
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
    /// Cast operation.
    CAST,
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
        Cast,
        Branch,
        Jump,
        Call,
        FuncReturn
    >;
    Data data;
};

struct ValueData {
    /// Type of value: array, pointer, funtion.
    Type ty;
    /// Name of value, null if no name.
    std::optional<std::string> name;
    /// Values that this Value is used by.
    Slice used_by;
    /// Kind of value.
    ValueKind kind;
};

}  // namespace rcc::ir