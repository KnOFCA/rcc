#include "IRBuilder.h"
#include "ir/koopa.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <functional>

namespace rcc::ir {

using namespace ast;
using namespace symtab;

namespace {

std::string extract_direct_decl_name(
    const std::shared_ptr<ast::DirectDeclarator>& direct) {
    if (!direct) return "";
    if (auto id = std::dynamic_pointer_cast<ast::DDIdentifier>(direct)) {
        return id->name;
    }
    if (auto arr = std::dynamic_pointer_cast<ast::DDArray>(direct)) {
        return extract_direct_decl_name(arr->base);
    }
    if (auto call = std::dynamic_pointer_cast<ast::DDCall>(direct)) {
        return extract_direct_decl_name(
            std::dynamic_pointer_cast<ast::DirectDeclarator>(call->base));
    }
    if (auto paren = std::dynamic_pointer_cast<ast::DDParen>(direct)) {
        auto innerDecl = std::dynamic_pointer_cast<ast::Declarator>(paren->inner);
        if (!innerDecl) return "";
        return extract_direct_decl_name(innerDecl->direct);
    }
    return "";
}

int32_t parse_char_literal_value(const std::string& raw) {
    if (raw.size() < 2 || raw.front() != '\'' || raw.back() != '\'') {
        return 0;
    }

    const std::string body = raw.substr(1, raw.size() - 2);
    if (body.empty()) return 0;

    if (body[0] != '\\') {
        return static_cast<unsigned char>(body[0]);
    }

    if (body.size() == 1) return 0;

    const char esc = body[1];
    switch (esc) {
        case 'a': return '\a';
        case 'b': return '\b';
        case 'f': return '\f';
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case 'v': return '\v';
        case '\\': return '\\';
        case '\'': return '\'';
        case '\"': return '\"';
        case '?': return '\?';
        case 'x': {
            if (body.size() < 3) return 0;
            int32_t value = 0;
            for (std::size_t i = 2; i < body.size(); ++i) {
                const char c = body[i];
                int digit = -1;
                if (c >= '0' && c <= '9') digit = c - '0';
                else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10;
                else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10;
                else break;
                value = (value << 4) | digit;
            }
            return value;
        }
        default: {
            // 八进制转义：最多 3 位
            if (esc >= '0' && esc <= '7') {
                int32_t value = esc - '0';
                std::size_t i = 2;
                std::size_t count = 1;
                while (i < body.size() && count < 3 &&
                       body[i] >= '0' && body[i] <= '7') {
                    value = (value << 3) | (body[i] - '0');
                    ++i;
                    ++count;
                }
                return value;
            }
            return static_cast<unsigned char>(esc);
        }
    }
}

} // namespace

// ============================================================================
// 程序构建入口
// ============================================================================

ErrorCode IRBuilder::build_from_AST(std::shared_ptr<TranslationUnit> node) {
    program = std::make_shared<Program>();
    
    // 遍历所有外部声明
    for (auto& ed : node->externalDecls) {
        if (auto decl = std::dynamic_pointer_cast<Declaration>(ed)) {
            auto val = build_global_declaration(decl);
            if (val) {
                program->values.buffer.push_back(val);
            }
        } else if (auto fdef = std::dynamic_pointer_cast<FunctionDef>(ed)) {
            auto func = build_function(fdef);
            if (func) {
                program->funcs.buffer.push_back(func);
            }
        }
    }
    
    return ErrorCode::SUCCESS;
}

// ============================================================================
// 函数定义
// ============================================================================

// TODO: fill param
Function IRBuilder::build_function(const std::shared_ptr<FunctionDef>& fdef) {
    if (!fdef) return nullptr;

    // 构建函数类型
    Type funcType = build_type(fdef->specs, fdef->declarator);
    if (!funcType) return nullptr;
    
    // 创建函数数据
    auto funcData = std::make_shared<FunctionData>();
    funcData->ty = funcType;
    currentFunc_ = funcData;
    
    // 提取函数名
    if (fdef->declarator && fdef->declarator->direct) {
        auto fnName = extract_direct_decl_name(fdef->declarator->direct);
        if (!fnName.empty()) {
            funcData->name = "@" + fnName;
        }
    }

    // 函数体构建前先在外层作用域注册函数，支持递归调用
    if (funcData->name) {
        std::string nm = funcData->name.value();
        if (!nm.empty() && nm[0] == '@') {
            nm = nm.substr(1);
        }
        auto sym = std::make_shared<symtab::Symbol>();
        sym->name = nm;
        sym->kind = symtab::SymbolKind::Function;
        sym->type = nullptr;
        sym->irFunction = funcData;
        symtab.define(sym);
    }

    // 进入函数体作用域
    symtab.enterScope();
    
    // 创建入口基本块
    BasicBlock entry = create_basic_block("entry");
    set_insert_point(entry);
    
    // 处理函数体
    if (auto body = std::dynamic_pointer_cast<CompoundStmt>(fdef->body)) {
        build_compound_stmt(body);
    }
    
    // 离开作用域
    symtab.leaveScope();
    
    currentFunc_ = nullptr;
    return funcData;
}

// ============================================================================
// 全局声明
// ============================================================================

Value IRBuilder::build_global_declaration(const std::shared_ptr<Declaration>& decl) {
    if (!decl || !decl->specs) return nullptr;
    
    for (auto& initDecl : decl->initDeclarators) {
        if (!initDecl || !initDecl->declarator) continue;
        
        // 构建类型
        Type ty = build_type(decl->specs, initDecl->declarator);
        if (!ty) continue;
        
        // 提取变量名
        auto symName = extract_direct_decl_name(initDecl->declarator->direct);
        std::string name = symName.empty() ? "" : "@" + symName;
        
        // 创建全局分配
        auto valData = std::make_shared<ValueData>();
        valData->ty = build_pointer_type(ty);
        valData->name = name;
        valData->kind.tag = ValueTag::GLOBAL_ALLOC;

        // 将全局变量加入符号表，供后续全局初始化表达式引用
        if (!symName.empty()) {
            auto sym = std::make_shared<symtab::Symbol>();
            sym->name = symName;
            sym->kind = symtab::SymbolKind::Object;
            sym->type = nullptr;
            sym->irValue = valData;
            symtab.define(sym);
        }
        
        // 处理初始化
        if (initDecl->initializer) {
            if (auto exprInit = std::dynamic_pointer_cast<ExprInitializer>(initDecl->initializer)) {
                valData->kind.data = GlobalAlloc{build_expr(
                    std::dynamic_pointer_cast<Expr>(exprInit->expr))};
            }
        } else {
            // 零初始化
            auto zeroInit = std::make_shared<ValueData>();
            zeroInit->kind.tag = ValueTag::ZERO_INIT;
            valData->kind.data = GlobalAlloc{zeroInit};
        }
        
        return valData;
    }
    
    return nullptr;
}

// ============================================================================
// 基本块管理
// ============================================================================

BasicBlock IRBuilder::create_basic_block(const std::string& name) {
    auto bb = std::make_shared<BasicBlockData>();
    if (!name.empty()) {
        bb->name = "%" + name;
    }
    if (currentFunc_) {
        currentFunc_->bbs.buffer.push_back(bb);
    }
    return bb;
}

void IRBuilder::set_insert_point(BasicBlock bb) {
    currentBB_ = bb;
}

// ============================================================================
// 复合语句
// ============================================================================

bool IRBuilder::current_block_terminated() const {
    if (!currentBB_ || currentBB_->insts.buffer.empty()) return false;
    auto last = std::get<Value>(currentBB_->insts.buffer.back());
    if (!last) return false;
    return last->kind.tag == ValueTag::JUMP ||
           last->kind.tag == ValueTag::BRANCH ||
           last->kind.tag == ValueTag::RETURN;
}

void IRBuilder::build_stmt(const std::shared_ptr<ast::Stmt>& stmt) {
    if (!stmt) return;

    if (auto comp = std::dynamic_pointer_cast<CompoundStmt>(stmt)) {
        build_compound_stmt(comp);
    } else if (auto exprStmt = std::dynamic_pointer_cast<ExprStmt>(stmt)) {
        build_expr(std::dynamic_pointer_cast<Expr>(exprStmt->expr));
    } else if (auto ifs = std::dynamic_pointer_cast<IfStmt>(stmt)) {
        build_if_stmt(ifs);
    } else if (auto fs = std::dynamic_pointer_cast<ForStmt>(stmt)) {
        build_for_stmt(fs);
    } else if (auto ws = std::dynamic_pointer_cast<WhileStmt>(stmt)) {
        build_while_stmt(ws);
    } else if (auto dws = std::dynamic_pointer_cast<DoWhileStmt>(stmt)) {
        // 保持原有语义近似处理，后续可替换为专用 do-while CFG
        auto fake = std::make_shared<WhileStmt>();
        fake->cond = dws->cond;
        fake->body = dws->body;
        build_while_stmt(fake);
    } else if (auto ss = std::dynamic_pointer_cast<SwitchStmt>(stmt)) {
        if (ss->expr) {
            build_expr(std::dynamic_pointer_cast<Expr>(ss->expr));
        }
        build_stmt(ss->stmt);
    } else if (auto ls = std::dynamic_pointer_cast<LabelStmt>(stmt)) {
        build_stmt(ls->stmt);
    } else if (auto cs = std::dynamic_pointer_cast<CaseStmt>(stmt)) {
        if (cs->expr) {
            build_expr(std::dynamic_pointer_cast<Expr>(cs->expr));
        }
        build_stmt(cs->stmt);
    } else if (auto ds = std::dynamic_pointer_cast<DefaultStmt>(stmt)) {
        build_stmt(ds->stmt);
    } else if (std::dynamic_pointer_cast<GotoStmt>(stmt)) {
        // TODO: 支持 goto/label 跳转
    } else if (std::dynamic_pointer_cast<BreakStmt>(stmt)) {
        build_break_stmt();
    } else if (std::dynamic_pointer_cast<ContinueStmt>(stmt)) {
        build_continue_stmt();
    } else if (auto rs = std::dynamic_pointer_cast<ReturnStmt>(stmt)) {
        build_return_stmt(rs);
    }
}

// 精确处理复合语句及其中的各种语句类型
void IRBuilder::build_compound_stmt(const std::shared_ptr<CompoundStmt>& comp) {
    if (!comp) return;

    symtab.enterScope();

    for (auto& item : comp->items) {
        if (auto declItem = std::dynamic_pointer_cast<DeclItem>(item)) {
            build_local_declaration(
                std::dynamic_pointer_cast<Declaration>(declItem->decl));
        } else if (auto stmtItem = std::dynamic_pointer_cast<StmtItem>(item)) {
            build_stmt(stmtItem->stmt);
        }
    }

    symtab.leaveScope();
}

// ============================================================================
// 控制流语句
// ============================================================================

void IRBuilder::build_if_stmt(const std::shared_ptr<IfStmt>& ifs) {
    if (!ifs) return;
    
    // 构建条件表达式
    Value cond = build_expr(std::dynamic_pointer_cast<Expr>(ifs->cond));
    
    // 创建基本块
    BasicBlock thenBB = create_basic_block("if.then");
    BasicBlock elseBB = ifs->elseStmt ? create_basic_block("if.else") : nullptr;
    BasicBlock mergeBB = create_basic_block("if.merge");
    
    // 条件分支
    if (elseBB) {
        build_cond_br(cond, thenBB, elseBB);
    } else {
        build_cond_br(cond, thenBB, mergeBB);
    }
    
    // Then 分支
    set_insert_point(thenBB);
    build_stmt(ifs->thenStmt);
    if (!current_block_terminated()) {
        build_br(mergeBB);
    }
    
    // Else 分支
    if (elseBB) {
        set_insert_point(elseBB);
        build_stmt(ifs->elseStmt);
        if (!current_block_terminated()) {
            build_br(mergeBB);
        }
    }
    
    set_insert_point(mergeBB);
}

void IRBuilder::build_while_stmt(const std::shared_ptr<WhileStmt>& ws) {
    if (!ws) return;
    
    BasicBlock condBB = create_basic_block("while.cond");
    BasicBlock bodyBB = create_basic_block("while.body");
    BasicBlock endBB = create_basic_block("while.end");
    
    // 保存 break/continue 目标
    breakStack_.push_back(endBB);
    continueStack_.push_back(condBB);
    
    build_br(condBB);
    
    // 条件块
    set_insert_point(condBB);
    Value cond = build_expr(std::dynamic_pointer_cast<Expr>(ws->cond));
    build_cond_br(cond, bodyBB, endBB);
    
    // 循环体
    set_insert_point(bodyBB);
    build_stmt(ws->body);
    if (!current_block_terminated()) {
        build_br(condBB);
    }
    
    // 结束块
    set_insert_point(endBB);
    
    breakStack_.pop_back();
    continueStack_.pop_back();
}

void IRBuilder::build_return_stmt(const std::shared_ptr<ReturnStmt>& rs) {
    if (!rs) {
        build_ret();
    } else {
        Value retVal = build_expr(std::dynamic_pointer_cast<Expr>(rs->expr));
        build_ret(retVal);
    }
}

// ============================================================================
// 表达式求值
// ============================================================================

Value IRBuilder::build_expr(const std::shared_ptr<Expr>& expr) {
    if (!expr) return nullptr;
    
    if (auto id = std::dynamic_pointer_cast<IdExpr>(expr)) {
        return build_id_expr(id);
    } else if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
        return build_literal_expr(lit);
    } else if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
        return build_binary_expr(bin);
    } else if (auto un = std::dynamic_pointer_cast<UnaryExpr>(expr)) {
        return build_unary_expr(un);
    } else if (auto post = std::dynamic_pointer_cast<PostfixExpr>(expr)) {
        return build_postfix_expr(post);
    } else if (auto call = std::dynamic_pointer_cast<CallExpr>(expr)) {
        return build_call_expr(call);
    } else if (auto cond = std::dynamic_pointer_cast<ConditionalExpr>(expr)) {
        return build_conditional_expr(cond);
    }
    
    return nullptr;
}

Value IRBuilder::build_binary_expr(const std::shared_ptr<BinaryExpr>& bin) {
    if (!bin) return nullptr;

    auto lhsExpr = std::dynamic_pointer_cast<Expr>(bin->lhs);
    auto rhsExpr = std::dynamic_pointer_cast<Expr>(bin->rhs);
    if (!lhsExpr || !rhsExpr) return nullptr;

    if (bin->op == opcode::ASSIGN ||
        bin->op == opcode::PLUSASSIGN ||
        bin->op == opcode::MINUSASSIGN ||
        bin->op == opcode::MULASSIGN ||
        bin->op == opcode::DIVASSIGN ||
        bin->op == opcode::MODASSIGN ||
        bin->op == opcode::ANDASSIGN ||
        bin->op == opcode::ORASSIGN ||
        bin->op == opcode::XORASSIGN ||
        bin->op == opcode::LSASSIGN ||
        bin->op == opcode::RSASSIGN) {
        Value addr = build_lvalue(lhsExpr);
        if (!addr) return nullptr;

        Value rhs = build_expr(rhsExpr);
        if (!rhs) return nullptr;

        if (bin->op == opcode::ASSIGN) {
            build_store(rhs, addr);
            return rhs;
        }

        BinaryOp bop = BinaryOp::ADD;
        switch (bin->op) {
            case opcode::PLUSASSIGN: bop = BinaryOp::ADD; break;
            case opcode::MINUSASSIGN: bop = BinaryOp::SUB; break;
            case opcode::MULASSIGN: bop = BinaryOp::MUL; break;
            case opcode::DIVASSIGN: bop = BinaryOp::DIV; break;
            case opcode::MODASSIGN: bop = BinaryOp::MOD; break;
            case opcode::ANDASSIGN: bop = BinaryOp::AND; break;
            case opcode::ORASSIGN:  bop = BinaryOp::OR; break;
            case opcode::XORASSIGN: bop = BinaryOp::XOR; break;
            case opcode::LSASSIGN:  bop = BinaryOp::SHL; break;
            case opcode::RSASSIGN:  bop = BinaryOp::SAR; break;
            default: break;
        }

        Value oldVal = build_load(addr);
        Value newVal = build_binary(bop, oldVal, rhs);
        build_store(newVal, addr);
        return newVal;
    }

    Value lhs = build_expr(lhsExpr);
    Value rhs = build_expr(rhsExpr);

    BinaryOp op;
    switch (bin->op) {
        case opcode::ADD: op = BinaryOp::ADD; break;
        case opcode::SUB: op = BinaryOp::SUB; break;
        case opcode::MUL: op = BinaryOp::MUL; break;
        case opcode::DIV: op = BinaryOp::DIV; break;
        case opcode::MOD: op = BinaryOp::MOD; break;
        case opcode::EQ:  op = BinaryOp::EQ; break;
        case opcode::NEQ: op = BinaryOp::NOT_EQ; break;
        case opcode::LT:  op = BinaryOp::LT; break;
        case opcode::GT:  op = BinaryOp::GT; break;
        case opcode::LE:  op = BinaryOp::LE; break;
        case opcode::GE:  op = BinaryOp::GE; break;
        case opcode::ANDAND: op = BinaryOp::AND; break;
        case opcode::OROR:   op = BinaryOp::OR; break;
        case opcode::BITAND: op = BinaryOp::AND; break;
        case opcode::BITOR:  op = BinaryOp::OR; break;
        case opcode::BITXOR: op = BinaryOp::XOR; break;
        case opcode::LSHIFT: op = BinaryOp::SHL; break;
        case opcode::RSHIFT: op = BinaryOp::SAR; break;
        default: return nullptr;
    }
    
    return build_binary(op, lhs, rhs);
}

Value IRBuilder::build_unary_expr(const std::shared_ptr<UnaryExpr>& un) {
    if (!un) return nullptr;
    
    Value operand = build_expr(std::dynamic_pointer_cast<Expr>(un->operand));
    
    switch (un->op) {
        case opcode::STAR:  // 解引用
            return build_load(operand);
        case opcode::AMP:   // 取地址
            return build_lvalue(std::dynamic_pointer_cast<Expr>(un->operand));
        case opcode::MINUSMINUS:
        case opcode::PLUSPLUS: {
            // 前缀 ++/--: 先修改再返回
            Value addr = build_lvalue(std::dynamic_pointer_cast<Expr>(un->operand));
            Value oldVal = build_load(addr);
            Value one = build_integer_const(1);
            Value newVal = build_binary(
                un->op == opcode::PLUSPLUS ? BinaryOp::ADD : BinaryOp::SUB,
                oldVal, one);
            build_store(newVal, addr);
            return newVal;
        }
        case opcode::BITNOT: {
            // ~x == x ^ -1
            Value allOnes = build_integer_const(-1);
            return build_binary(BinaryOp::XOR, operand, allOnes);
        }
        default: break;
    }
    
    return nullptr;
}

Value IRBuilder::build_postfix_expr(const std::shared_ptr<PostfixExpr>& post) {
    if (!post) return nullptr;

    if (post->op == opcode::ARRAY) {
        auto baseExpr = std::dynamic_pointer_cast<Expr>(post->array);
        auto idxExpr = std::dynamic_pointer_cast<Expr>(post->expr);
        if (!baseExpr || !idxExpr) return nullptr;

        Value base = build_lvalue(baseExpr);
        Value idx = build_expr(idxExpr);
        if (!base || !idx) return nullptr;

        auto ptr = std::make_shared<ValueData>();
        ptr->name = "%" + std::to_string(tempCounter_++);
        ptr->kind.tag = ValueTag::GET_PTR;
        ptr->kind.data = GetPtr{base, idx};
        if (currentBB_) {
            currentBB_->insts.buffer.push_back(ptr);
        }
        return build_load(ptr);
    }

    if (post->op == opcode::PLUSPLUS || post->op == opcode::MINUSMINUS) {
        auto target = std::dynamic_pointer_cast<Expr>(post->expr);
        Value addr = build_lvalue(target);
        Value oldVal = build_load(addr);
        Value one = build_integer_const(1);
        Value newVal = build_binary(
            post->op == opcode::PLUSPLUS ? BinaryOp::ADD : BinaryOp::SUB,
            oldVal, one);
        build_store(newVal, addr);
        return oldVal;
    }

    return nullptr;
}

Value IRBuilder::build_call_expr(const std::shared_ptr<CallExpr>& call) {
    if (!call) return nullptr;
    
    // 查找被调用函数
    std::string funcName;
    if (auto id = std::dynamic_pointer_cast<IdExpr>(call->callee)) {
        funcName = id->name;
    }
    
    // 构建参数列表
    std::vector<Value> args;
    for (auto& arg : call->args) {
        args.push_back(build_expr(std::dynamic_pointer_cast<Expr>(arg)));
    }
    
    // 查找函数对象
    Function callee = nullptr;
    if (!funcName.empty()) {
        auto sym = symtab.lookup(funcName);
        if (sym && sym->irFunction) {
            callee = sym->irFunction;
        }
    }
    // 使用辅助接口创建调用
    return build_call(callee, args);
}

Value IRBuilder::build_id_expr(const std::shared_ptr<IdExpr>& id) {
    if (!id) return nullptr;
    
    // 从符号表查找
    auto sym = symtab.lookup(id->name);
    if (!sym) {
        std::cerr << "Error: undefined identifier: " << id->name << std::endl;
        return nullptr;
    }
    
    // 加载变量值
    Value addr = sym->irValue;
    if (!addr) {
        std::cerr << "Error: symbol has no associated address: " << id->name << std::endl;
        return nullptr;
    }
    return build_load(addr);
}

// ============================================================================
// 新增函数实现
// ============================================================================

Value IRBuilder::build_local_declaration(const std::shared_ptr<Declaration>& decl) {
    if (!decl || !decl->specs) return nullptr;

    for (auto& initDecl : decl->initDeclarators) {
        if (!initDecl || !initDecl->declarator) continue;

        // 构建变量类型
        Type ty = build_type(decl->specs, initDecl->declarator);
        if (!ty) continue;

        // 提取名字
        std::string name = extract_direct_decl_name(initDecl->declarator->direct);

        // 为局部变量分配空间
        Value addr = build_alloca(ty);
        if (!name.empty()) {
            addr->name = "%" + name;
        }

        // 将地址存入符号表
        if (!name.empty()) {
            auto sym = std::make_shared<symtab::Symbol>();
            sym->name = name;
            sym->kind = symtab::SymbolKind::Object;
            sym->type = nullptr; // 暂时不使用类型信息
            sym->irValue = addr;
            symtab.define(sym);
        }

        // 处理初始化
        if (initDecl->initializer) {
            if (auto exprInit = std::dynamic_pointer_cast<ExprInitializer>(initDecl->initializer)) {
                Value initVal = build_expr(std::dynamic_pointer_cast<Expr>(exprInit->expr));
                if (initVal)
                    build_store(initVal, addr);
            }
        }
    }

    return nullptr;
}


void IRBuilder::build_for_stmt(const std::shared_ptr<ForStmt>& fs) {
    if (!fs) return;

    // init 部分
    if (fs->init) {
        if (auto decl = std::dynamic_pointer_cast<Declaration>(fs->init)) {
            build_local_declaration(decl);
        } else if (auto exprStmt = std::dynamic_pointer_cast<ExprStmt>(fs->init)) {
            build_expr(std::dynamic_pointer_cast<Expr>(exprStmt->expr));
        }
    }

    BasicBlock condBB = create_basic_block("for.cond");
    BasicBlock bodyBB = create_basic_block("for.body");
    BasicBlock iterBB = create_basic_block("for.iter");
    BasicBlock endBB = create_basic_block("for.end");

    breakStack_.push_back(endBB);
    continueStack_.push_back(iterBB);

    // 直接跳转到条件判断
    build_br(condBB);

    // 条件块
    set_insert_point(condBB);
    if (fs->cond) {
        if (auto stmt = std::dynamic_pointer_cast<ExprStmt>(fs->cond)) {
            Value condVal = build_expr(std::dynamic_pointer_cast<Expr>(stmt->expr));
            if (condVal)
                build_cond_br(condVal, bodyBB, endBB);
            else
                build_br(bodyBB);
        } else {
            build_br(bodyBB);
        }
    } else {
        build_br(bodyBB);
    }

    // 主体
    set_insert_point(bodyBB);
    if (fs->body) {
        build_stmt(fs->body);
    }
    if (!current_block_terminated()) {
        build_br(iterBB);
    }

    // 迭代表达式
    set_insert_point(iterBB);
    if (fs->iter) {
        build_expr(std::dynamic_pointer_cast<Expr>(fs->iter));
    }
    build_br(condBB);

    // 结束
    set_insert_point(endBB);
    breakStack_.pop_back();
    continueStack_.pop_back();
}

void IRBuilder::build_break_stmt() {
    if (!breakStack_.empty()) {
        build_br(breakStack_.back());
    }
}

void IRBuilder::build_continue_stmt() {
    if (!continueStack_.empty()) {
        build_br(continueStack_.back());
    }
}

Value IRBuilder::build_conditional_expr(const std::shared_ptr<ConditionalExpr>& ce) {
    if (!ce) return nullptr;

    Value cond = build_expr(std::dynamic_pointer_cast<Expr>(ce->cond));
    BasicBlock thenBB = create_basic_block("cond.then");
    BasicBlock elseBB = create_basic_block("cond.else");
    BasicBlock mergeBB = create_basic_block("cond.merge");

    // 为结果准备一个临时地址，类型稍后填充
    Value resultAddr = build_alloca(nullptr);

    build_cond_br(cond, thenBB, elseBB);

    // then
    set_insert_point(thenBB);
    Value thenVal = build_expr(std::dynamic_pointer_cast<Expr>(ce->thenExpr));
    if (resultAddr && thenVal && resultAddr->ty) {
        auto ptr = std::get<std::shared_ptr<TypeKind::pointer>>(resultAddr->ty->data);
        if (ptr && !ptr->base) ptr->base = thenVal->ty;
    }
    build_store(thenVal, resultAddr);
    build_br(mergeBB);

    // else
    set_insert_point(elseBB);
    Value elseVal = build_expr(std::dynamic_pointer_cast<Expr>(ce->elseExpr));
    if (resultAddr && elseVal && resultAddr->ty) {
        auto ptr = std::get<std::shared_ptr<TypeKind::pointer>>(resultAddr->ty->data);
        if (ptr && !ptr->base) ptr->base = elseVal->ty;
    }
    build_store(elseVal, resultAddr);
    build_br(mergeBB);

    // merge
    set_insert_point(mergeBB);
    return build_load(resultAddr);
}

Value IRBuilder::build_lvalue(const std::shared_ptr<ast::Expr>& expr) {
    if (!expr) return nullptr;

    if (auto id = std::dynamic_pointer_cast<IdExpr>(expr)) {
        auto sym = symtab.lookup(id->name);
        if (!sym) {
            std::cerr << "Error: undefined lvalue identifier: " << id->name << std::endl;
            return nullptr;
        }
        return sym->irValue;
    }

    if (auto un = std::dynamic_pointer_cast<UnaryExpr>(expr)) {
        if (un->op == opcode::STAR) {
            // *p 的 lvalue 就是指针本身
            return build_expr(std::dynamic_pointer_cast<Expr>(un->operand));
        }
    }

    if (auto post = std::dynamic_pointer_cast<PostfixExpr>(expr)) {
        if (post->op == opcode::ARRAY) {
            auto baseExpr = std::dynamic_pointer_cast<Expr>(post->array);
            auto idxExpr = std::dynamic_pointer_cast<Expr>(post->expr);
            if (!baseExpr || !idxExpr) return nullptr;

            Value base = build_lvalue(baseExpr);
            Value idx = build_expr(idxExpr);
            if (!base || !idx) return nullptr;

            auto valData = std::make_shared<ValueData>();
            valData->name = "%" + std::to_string(tempCounter_++);
            valData->kind.tag = ValueTag::GET_PTR;
            valData->kind.data = GetPtr{base, idx};
            if (currentBB_) currentBB_->insts.buffer.push_back(valData);
            return valData;
        }
    }

    if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(expr)) {
        if (bin->op == opcode::ARRAY) {
            // 数组索引：生成 get_ptr
            Value base = build_expr(std::dynamic_pointer_cast<Expr>(bin->lhs));
            Value idx = build_expr(std::dynamic_pointer_cast<Expr>(bin->rhs));
            auto valData = std::make_shared<ValueData>();
            valData->name = "%" + std::to_string(tempCounter_++);
            valData->kind.tag = ValueTag::GET_PTR;
            valData->kind.data = GetPtr{base, idx};
            if (currentBB_) currentBB_->insts.buffer.push_back(valData);
            return valData;
        }
    }

    return nullptr;
}

Value IRBuilder::build_literal_expr(const std::shared_ptr<LiteralExpr>& lit) {
    if (!lit) return nullptr;
    
    auto valData = std::make_shared<ValueData>();
    valData->kind.tag = ValueTag::NUMBER;
    
    Number num;
    switch (lit->type) {
        case LiteralExpr::INTEGER: {
            int32_t val = std::stoi(lit->value);
            num.num = Integer32{val};
            valData->ty = std::make_shared<TypeKind>();
            valData->ty->tag = TypeTag::INT32;
            break;
        }
        case LiteralExpr::FLOAT: {
            float val = std::stof(lit->value);
            num.num = Float{val};
            valData->ty = std::make_shared<TypeKind>();
            valData->ty->tag = TypeTag::FLOAT;
            break;
        }
        case LiteralExpr::CHAR: {
            int32_t val = parse_char_literal_value(lit->value);
            num.num = Integer32{val};
            valData->ty = std::make_shared<TypeKind>();
            valData->ty->tag = TypeTag::INT32;
            break;
        }
        default: break;
    }
    
    valData->kind.data = num;
    return valData;
}

// ============================================================================
// 内存操作
// ============================================================================

Value IRBuilder::build_alloca(Type ty) {
    auto valData = std::make_shared<ValueData>();
    valData->name = "%" + std::to_string(tempCounter_++);
    valData->ty = build_pointer_type(ty);
    valData->kind.tag = ValueTag::ALLOC;
    
    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
    
    return valData;
}

Value IRBuilder::build_load(Value ptr) {
    if (!ptr) return nullptr;
    
    auto valData = std::make_shared<ValueData>();
    valData->name = "%" + std::to_string(tempCounter_++);
    valData->kind.tag = ValueTag::LOAD;
    valData->kind.data = Load{ptr};
    
    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
    
    return valData;
}

void IRBuilder::build_store(Value value, Value ptr) {
    if (!value || !ptr) return;
    
    auto valData = std::make_shared<ValueData>();
    valData->kind.tag = ValueTag::STORE;
    valData->kind.data = Store{value, ptr};
    
    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
}

Value IRBuilder::build_binary(BinaryOp op, Value lhs, Value rhs) {
    if (!lhs || !rhs) return nullptr;
    
    auto valData = std::make_shared<ValueData>();
    valData->name = "%" + std::to_string(tempCounter_++);
    valData->kind.tag = ValueTag::BINARY;
    valData->kind.data = Binary{op, lhs, rhs};
    
    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
    
    return valData;
}

void IRBuilder::build_br(BasicBlock target) {
    auto valData = std::make_shared<ValueData>();
    valData->kind.tag = ValueTag::JUMP;
    valData->kind.data = Jump{target, {}};
    
    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
}

void IRBuilder::build_cond_br(Value cond, BasicBlock trueBB, BasicBlock falseBB) {
    if (!cond) return;
    
    auto valData = std::make_shared<ValueData>();
    valData->kind.tag = ValueTag::BRANCH;
    valData->kind.data = Branch{cond, trueBB, falseBB, {}, {}};
    
    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
}

void IRBuilder::build_ret(Value value) {
    auto valData = std::make_shared<ValueData>();
    valData->kind.tag = ValueTag::RETURN;
    valData->kind.data = FuncReturn{value};
    
    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
}

// ============================================================================
// 类型构建
// ============================================================================

Type IRBuilder::build_type(const std::shared_ptr<DeclSpec>& specs,
                            const std::shared_ptr<Declarator>& decl) {
    if (!specs) return nullptr;
    
    // 解析基础类型
    Type baseType = std::make_shared<TypeKind>();
    
    for (auto& spec : specs->specs) {
        if (auto bt = std::dynamic_pointer_cast<BuiltinTypeSpec>(spec)) {
            switch (bt->builtin) {
                case BuiltinTypeSpec::Builtin::Int:
                    baseType->tag = TypeTag::INT32; break;
                case BuiltinTypeSpec::Builtin::Float:
                    baseType->tag = TypeTag::FLOAT; break;
                case BuiltinTypeSpec::Builtin::Double:
                    baseType->tag = TypeTag::DOUBLE; break;
                case BuiltinTypeSpec::Builtin::Char:
                    baseType->tag = TypeTag::INT8; break;
                case BuiltinTypeSpec::Builtin::Void:
                    baseType->tag = TypeTag::UNIT; break;
                default: break;
            }
        }
    }
    
    if (!decl) return baseType;

    struct TypeCtor {
        enum class Kind { Pointer, Array, Function };
        Kind kind;
        std::size_t arraySize = 0;
        std::vector<Type> params;
    };

    std::vector<TypeCtor> ctors;
    std::function<void(const std::shared_ptr<Declarator>&)> collect_from_decl;
    std::function<void(const std::shared_ptr<DirectDeclarator>&)> collect_from_direct;

    collect_from_decl = [&](const std::shared_ptr<Declarator>& d) {
        if (!d) return;
        collect_from_direct(d->direct);
        auto ptr = d->pointer;
        while (ptr) {
            ctors.push_back(TypeCtor{TypeCtor::Kind::Pointer, 0, {}});
            ptr = ptr->next;
        }
    };

    collect_from_direct = [&](const std::shared_ptr<DirectDeclarator>& direct) {
        if (!direct) return;

        if (auto ddArray = std::dynamic_pointer_cast<DDArray>(direct)) {
            collect_from_direct(ddArray->base);
            std::size_t size = 0;
            if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(ddArray->size)) {
                if (lit->type == LiteralExpr::INTEGER) {
                    size = std::stoul(lit->value);
                }
            }
            ctors.push_back(TypeCtor{TypeCtor::Kind::Array, size, {}});
            return;
        }

        if (auto ddCall = std::dynamic_pointer_cast<DDCall>(direct)) {
            collect_from_direct(std::dynamic_pointer_cast<DirectDeclarator>(ddCall->base));
            std::vector<Type> params;
            params.reserve(ddCall->params.size());
            for (const auto& p : ddCall->params) {
                auto pDecl = std::dynamic_pointer_cast<Declarator>(p->declarator);
                params.push_back(build_type(p->specs, pDecl));
            }
            TypeCtor ctor{TypeCtor::Kind::Function, 0, {}};
            ctor.params = std::move(params);
            ctors.push_back(std::move(ctor));
            return;
        }

        if (auto ddParen = std::dynamic_pointer_cast<DDParen>(direct)) {
            auto innerDecl = std::dynamic_pointer_cast<Declarator>(ddParen->inner);
            collect_from_decl(innerDecl);
            return;
        }
    };

    collect_from_decl(decl);

    Type result = baseType;
    for (auto it = ctors.rbegin(); it != ctors.rend(); ++it) {
        switch (it->kind) {
            case TypeCtor::Kind::Pointer:
                result = build_pointer_type(result);
                break;
            case TypeCtor::Kind::Array:
                result = build_array_type(result, it->arraySize);
                break;
            case TypeCtor::Kind::Function:
                result = build_function_type(result, it->params);
                break;
        }
    }
    return result;
}

Type IRBuilder::build_pointer_type(Type base) {
    auto ty = std::make_shared<TypeKind>();
    ty->tag = TypeTag::POINTER;
    ty->data = std::make_shared<TypeKind::pointer>();
    std::get<std::shared_ptr<TypeKind::pointer>>(ty->data)->base = base;
    return ty;
}

Type IRBuilder::build_array_type(Type base, std::size_t size) {
    auto ty = std::make_shared<TypeKind>();
    ty->tag = TypeTag::ARRAY;
    ty->data = std::make_shared<TypeKind::array>();
    std::get<std::shared_ptr<TypeKind::array>>(ty->data)->base = base;
    std::get<std::shared_ptr<TypeKind::array>>(ty->data)->len = size;
    return ty;
}

// ============================================================================
// 常量构建
// ============================================================================

Type IRBuilder::build_function_type(Type ret, const std::vector<Type>& params) {
    auto ty = std::make_shared<TypeKind>();
    ty->tag = TypeTag::FUNCTION;
    ty->data = std::make_shared<TypeKind::function>();
    auto funcData = std::get<std::shared_ptr<TypeKind::function>>(ty->data);
    funcData->ret = ret;
    funcData->params.kind = SliceItemKind::TYPE;
    for (auto &p : params) {
        funcData->params.buffer.push_back(p);
    }
    return ty;
}

Value IRBuilder::build_integer_const(int32_t value) {
    auto valData = std::make_shared<ValueData>();
    valData->kind.tag = ValueTag::NUMBER;

    Number num;
    num.num = Integer32{value};
    valData->kind.data = num;

    valData->ty = std::make_shared<TypeKind>();
    valData->ty->tag = TypeTag::INT32;

    return valData;
}

Value IRBuilder::build_float_const(float value) {
    auto valData = std::make_shared<ValueData>();
    valData->kind.tag = ValueTag::NUMBER;

    Number num;
    num.num = Float{value};
    valData->kind.data = num;

    valData->ty = std::make_shared<TypeKind>();
    valData->ty->tag = TypeTag::FLOAT;

    return valData;
}

Value IRBuilder::build_call(Function func, const std::vector<Value>& args) {
    if (!func) return nullptr;

    auto callData = Call{};
    callData.callee = func;
    callData.args.kind = SliceItemKind::VALUE;
    for (auto &a : args) {
        callData.args.buffer.push_back(a);
    }

    auto valData = std::make_shared<ValueData>();
    valData->kind.tag = ValueTag::CALL;
    valData->kind.data = callData;

    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
    return valData;
}

} // namespace rcc::ir
