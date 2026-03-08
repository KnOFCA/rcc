#pragma once

#include "koopa.h"
#include "front/AST.h"
#include "front/ASTVisitor.h"
#include "front/FrontSymtab.h"

#include <filesystem>
#include <span>
#include <unordered_map>

namespace rcc::ir {

class IRBuilder : public front::ASTVisitor<IRBuilder> {
  public:
    struct SwitchContext {
        BasicBlock endBB;
        std::unordered_map<const ast::CaseStmt*, BasicBlock> caseBBs;
        std::unordered_map<const ast::DefaultStmt*, BasicBlock> defaultBBs;
    };

    std::shared_ptr<Program> program;
    symtab::SymbolTable symtab;
    Function currentFunc_;
    BasicBlock currentBB_;
    std::vector<BasicBlock> breakStack_;
    std::vector<BasicBlock> continueStack_;
    std::vector<SwitchContext> switchStack_;
    std::unordered_map<std::string, BasicBlock> labelBBs_;
    std::unordered_map<std::string, std::size_t> bbNameCounters_;
    long tempCounter_ = 0;
    
    // 从 AST 生成 IR 程序
    ErrorCode build_from_AST(std::shared_ptr<ast::TranslationUnit> node);

    // 程序生命周期管理
    void delete_program();
    std::shared_ptr<Program> get_program() const;

    // 处理函数定义
    Function build_function(const std::shared_ptr<ast::FunctionDef>& fdef);

    // 处理全局声明/变量
    Value build_global_declaration(const std::shared_ptr<ast::Declaration>& decl);

    // 处理局部声明
    Value build_local_declaration(const std::shared_ptr<ast::Declaration>& decl);

    // 基本块管理
    BasicBlock create_basic_block(const std::string& name = "");
    void set_insert_point(BasicBlock bb);

    // 复合语句
    void build_stmt(const std::shared_ptr<ast::Stmt>& stmt);
    void build_compound_stmt(const std::shared_ptr<ast::CompoundStmt>& comp);
    bool current_block_terminated() const;

    // 控制流语句
    void build_if_stmt(const std::shared_ptr<ast::IfStmt>& ifs);
    void build_switch_stmt(const std::shared_ptr<ast::SwitchStmt>& ss);
    void build_while_stmt(const std::shared_ptr<ast::WhileStmt>& ws);
    void build_for_stmt(const std::shared_ptr<ast::ForStmt>& fs);
    void build_return_stmt(const std::shared_ptr<ast::ReturnStmt>& rs);
    void build_break_stmt();
    void build_continue_stmt();

    // 表达式求值，返回生成的 Value
    Value build_expr(const std::shared_ptr<ast::Expr>& expr);

    // 表达式
    Value build_binary_expr(const std::shared_ptr<ast::BinaryExpr>& bin);
    Value build_unary_expr(const std::shared_ptr<ast::UnaryExpr>& un);
    Value build_postfix_expr(const std::shared_ptr<ast::PostfixExpr>& post);
    Value build_call_expr(const std::shared_ptr<ast::CallExpr>& call);
    Value build_id_expr(const std::shared_ptr<ast::IdExpr>& id);
    Value build_literal_expr(const std::shared_ptr<ast::LiteralExpr>& lit);
    Value build_conditional_expr(const std::shared_ptr<ast::ConditionalExpr>& ce);
    Value build_cast_expr(const std::shared_ptr<ast::CastExpr>& cast);

    // 左值处理（用于赋值）
    Value build_lvalue(const std::shared_ptr<ast::Expr>& expr);

    // 从 AST 类型说明符生成 Koopa 类型
    Type build_type(const std::shared_ptr<ast::DeclSpec>& specs, 
                    const std::shared_ptr<ast::Declarator>& decl);

    // 处理指针类型
    Type build_pointer_type(Type base);

    // 处理数组类型
    Type build_array_type(Type base, std::size_t size);

    // 处理函数类型
    Type build_function_type(Type ret, const std::vector<Type>& params);

    // 常量生成
    Value build_integer_const(int32_t value);
    Value build_float_const(float value);

    // 内存操作
    Value build_alloca(Type ty);
    Value build_load(Value ptr);
    void build_store(Value value, Value ptr);
    Value build_get_ptr(Value src, Value index);

    // 二元运算
    Value build_binary(BinaryOp op, Value lhs, Value rhs);
    Value build_cast(CastOp op, Value value, Type targetTy);

    // 控制流
    void build_br(BasicBlock target);
    void build_cond_br(Value cond, BasicBlock true_bb, BasicBlock false_bb);

    // 函数调用
    Value build_call(Function func, const std::vector<Value>& args);

    // 返回
    void build_ret(Value value = nullptr);

    ErrorCode dump_to_file(const std::filesystem::path& path);
    ErrorCode dump_to_stdout();
    ErrorCode dump_llvm_to_file(const std::filesystem::path& path);
};

}
