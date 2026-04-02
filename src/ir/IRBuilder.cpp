#include "IRBuilder.h"
#include "ir/koopa.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <algorithm>
#include <cctype>

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

std::vector<std::shared_ptr<ast::ParameterDecl>> extract_function_params(
    const std::shared_ptr<ast::DirectDeclarator>& direct) {
    if (!direct) return {};
    if (auto call = std::dynamic_pointer_cast<ast::DDCall>(direct)) {
        return call->params;
    }
    if (auto arr = std::dynamic_pointer_cast<ast::DDArray>(direct)) {
        return extract_function_params(arr->base);
    }
    if (auto paren = std::dynamic_pointer_cast<ast::DDParen>(direct)) {
        auto innerDecl = std::dynamic_pointer_cast<ast::Declarator>(paren->inner);
        if (!innerDecl) return {};
        return extract_function_params(innerDecl->direct);
    }
    return {};
}

bool direct_declarator_has_varargs(
    const std::shared_ptr<ast::DirectDeclarator>& direct) {
    if (!direct) return false;
    if (auto call = std::dynamic_pointer_cast<ast::DDCall>(direct)) {
        return call->hasVarArgs;
    }
    if (auto arr = std::dynamic_pointer_cast<ast::DDArray>(direct)) {
        return direct_declarator_has_varargs(arr->base);
    }
    if (auto paren = std::dynamic_pointer_cast<ast::DDParen>(direct)) {
        auto innerDecl = std::dynamic_pointer_cast<ast::Declarator>(paren->inner);
        if (!innerDecl) return false;
        return direct_declarator_has_varargs(innerDecl->direct);
    }
    return false;
}

const std::shared_ptr<ir::TypeKind::function> extract_function_type(ir::Type type) {
    if (!type || type->tag != ir::TypeTag::FUNCTION) {
        return nullptr;
    }
    return std::get<std::shared_ptr<ir::TypeKind::function>>(type->data);
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

bool is_integer_type(Type ty) {
    if (!ty) return false;
    switch (ty->tag) {
        case TypeTag::INT8:
        case TypeTag::INT16:
        case TypeTag::INT32:
        case TypeTag::INT64:
            return true;
        default:
            return false;
    }
}

bool is_float_type(Type ty) {
    if (!ty) return false;
    return ty->tag == TypeTag::FLOAT || ty->tag == TypeTag::DOUBLE;
}

Value make_number_value(const Number::number& num, TypeTag tag) {
    auto value = std::make_shared<ValueData>();
    value->kind.tag = ValueTag::NUMBER;
    value->kind.data = Number{num};
    value->ty = std::make_shared<TypeKind>();
    value->ty->tag = tag;
    return value;
}

Value make_zero_init_value() {
    auto value = std::make_shared<ValueData>();
    value->kind.tag = ValueTag::ZERO_INIT;
    return value;
}

Type make_scalar_type(TypeTag tag) {
    auto ty = std::make_shared<TypeKind>();
    ty->tag = tag;
    return ty;
}

std::optional<long double> number_to_long_double(const Number::number& num) {
    if (std::holds_alternative<Integer8>(num)) return std::get<Integer8>(num).value;
    if (std::holds_alternative<Integer16>(num)) return std::get<Integer16>(num).value;
    if (std::holds_alternative<Integer32>(num)) return std::get<Integer32>(num).value;
    if (std::holds_alternative<Integer64>(num)) return std::get<Integer64>(num).value;
    if (std::holds_alternative<Float>(num)) return std::get<Float>(num).value;
    if (std::holds_alternative<Double>(num)) return std::get<Double>(num).value;
    return std::nullopt;
}

Value convert_constant_number(const Number::number& num, Type ty) {
    if (!ty) return nullptr;

    auto as_ld = number_to_long_double(num);
    if (!as_ld.has_value()) return nullptr;

    switch (ty->tag) {
        case TypeTag::INT8:
            return make_number_value(Integer8{static_cast<int8_t>(*as_ld)}, TypeTag::INT8);
        case TypeTag::INT16:
            return make_number_value(Integer16{static_cast<int16_t>(*as_ld)}, TypeTag::INT16);
        case TypeTag::INT32:
            return make_number_value(Integer32{static_cast<int32_t>(*as_ld)}, TypeTag::INT32);
        case TypeTag::INT64:
            return make_number_value(Integer64{static_cast<int64_t>(*as_ld)}, TypeTag::INT64);
        case TypeTag::FLOAT:
            return make_number_value(Float{static_cast<float>(*as_ld)}, TypeTag::FLOAT);
        case TypeTag::DOUBLE:
            return make_number_value(Double{static_cast<double>(*as_ld)}, TypeTag::DOUBLE);
        default:
            return nullptr;
    }
}

Value fold_constant_expr(const std::shared_ptr<Expr>& expr, Type targetTy) {
    if (!expr) return nullptr;

    if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr)) {
        switch (lit->type) {
            case LiteralExpr::INTEGER:
                return convert_constant_number(Number::number{Integer32{static_cast<int32_t>(std::stoi(lit->value))}},
                                               targetTy ? targetTy : make_scalar_type(TypeTag::INT32));
            case LiteralExpr::FLOAT:
                return convert_constant_number(Number::number{Float{std::stof(lit->value)}},
                                               targetTy ? targetTy : make_scalar_type(TypeTag::FLOAT));
            case LiteralExpr::CHAR:
                return convert_constant_number(Number::number{Integer32{parse_char_literal_value(lit->value)}},
                                               targetTy ? targetTy : make_scalar_type(TypeTag::INT32));
            default:
                return nullptr;
        }
    }

    if (auto cast = std::dynamic_pointer_cast<CastExpr>(expr)) {
        Type dstTy = nullptr;
        if (cast->type) {
            IRBuilder temp_builder;
            dstTy = temp_builder.build_type(cast->type->specs, nullptr);
        }
        auto inner = fold_constant_expr(std::dynamic_pointer_cast<Expr>(cast->expr), dstTy);
        if (!inner) return nullptr;
        if (!targetTy) return inner;
        const auto& number = std::get<Number>(inner->kind.data);
        return convert_constant_number(number.num, targetTy);
    }

    return nullptr;
}

Value build_constant_initializer(const std::shared_ptr<Initializer>& init,
                                 Type ty) {
    if (!ty) return nullptr;
    if (!init) return make_zero_init_value();

    if (ty->tag == TypeTag::ARRAY) {
        auto arrTy = std::get<std::shared_ptr<TypeKind::array>>(ty->data);
        if (!arrTy || !arrTy->base) return nullptr;

        auto aggregate = std::make_shared<ValueData>();
        aggregate->kind.tag = ValueTag::AGGREGATE;
        Aggregate agg;
        agg.elems.kind = SliceItemKind::VALUE;

        if (auto list = std::dynamic_pointer_cast<InitList>(init)) {
            const std::size_t initCount = std::min(arrTy->len, list->elements.size());
            for (std::size_t i = 0; i < initCount; ++i) {
                agg.elems.buffer.push_back(build_constant_initializer(list->elements[i], arrTy->base));
            }
            for (std::size_t i = initCount; i < arrTy->len; ++i) {
                agg.elems.buffer.push_back(make_zero_init_value());
            }
        } else {
            for (std::size_t i = 0; i < arrTy->len; ++i) {
                agg.elems.buffer.push_back(make_zero_init_value());
            }
        }

        aggregate->kind.data = agg;
        return aggregate;
    }

    if (auto exprInit = std::dynamic_pointer_cast<ExprInitializer>(init)) {
        if (auto folded = fold_constant_expr(std::dynamic_pointer_cast<Expr>(exprInit->expr), ty)) {
            return folded;
        }
    }

    if (auto list = std::dynamic_pointer_cast<InitList>(init)) {
        if (!list->elements.empty()) {
            return build_constant_initializer(list->elements.front(), ty);
        }
        return make_zero_init_value();
    }

    return nullptr;
}

Type pointee_type(Type ptrTy) {
    if (!ptrTy || ptrTy->tag != TypeTag::POINTER) return nullptr;
    auto ptr = std::get<std::shared_ptr<TypeKind::pointer>>(ptrTy->data);
    if (!ptr) return nullptr;
    return ptr->base;
}

std::shared_ptr<ast::Declarator> declarator_from_parameter_ast(const ast::AST& node) {
    if (!node) return nullptr;
    if (auto decl = std::dynamic_pointer_cast<ast::Declarator>(node)) {
        return decl;
    }
    if (auto absDecl = std::dynamic_pointer_cast<ast::AbstractDeclarator>(node)) {
        auto decl = std::make_shared<ast::Declarator>();
        decl->pointer = absDecl->pointer;
        return decl;
    }
    return nullptr;
}

bool is_char_array_type(Type ty) {
    if (!ty || ty->tag != TypeTag::ARRAY) return false;
    auto arr = std::get<std::shared_ptr<TypeKind::array>>(ty->data);
    return arr && arr->base && arr->base->tag == TypeTag::INT8;
}

std::vector<int32_t> parse_string_literal_bytes(const std::string& raw) {
    std::vector<int32_t> bytes;
    if (raw.size() < 2 || raw.front() != '"' || raw.back() != '"') return bytes;

    const std::string body = raw.substr(1, raw.size() - 2);
    for (std::size_t i = 0; i < body.size(); ++i) {
        char c = body[i];
        if (c != '\\') {
            bytes.push_back(static_cast<unsigned char>(c));
            continue;
        }

        if (i + 1 >= body.size()) {
            bytes.push_back(0);
            break;
        }

        char esc = body[++i];
        switch (esc) {
            case 'a': bytes.push_back('\a'); break;
            case 'b': bytes.push_back('\b'); break;
            case 'f': bytes.push_back('\f'); break;
            case 'n': bytes.push_back('\n'); break;
            case 'r': bytes.push_back('\r'); break;
            case 't': bytes.push_back('\t'); break;
            case 'v': bytes.push_back('\v'); break;
            case '\\': bytes.push_back('\\'); break;
            case '\'': bytes.push_back('\''); break;
            case '\"': bytes.push_back('\"'); break;
            case '?': bytes.push_back('\?'); break;
            case 'x': {
                int32_t value = 0;
                bool hasHex = false;
                while (i + 1 < body.size() && std::isxdigit(static_cast<unsigned char>(body[i + 1]))) {
                    const char hx = body[++i];
                    hasHex = true;
                    if (hx >= '0' && hx <= '9') value = (value << 4) | (hx - '0');
                    else if (hx >= 'a' && hx <= 'f') value = (value << 4) | (hx - 'a' + 10);
                    else value = (value << 4) | (hx - 'A' + 10);
                }
                bytes.push_back(hasHex ? value : 0);
                break;
            }
            default: {
                if (esc >= '0' && esc <= '7') {
                    int32_t value = esc - '0';
                    std::size_t count = 1;
                    while (i + 1 < body.size() && count < 3 &&
                           body[i + 1] >= '0' && body[i + 1] <= '7') {
                        value = (value << 3) | (body[++i] - '0');
                        ++count;
                    }
                    bytes.push_back(value);
                } else {
                    bytes.push_back(static_cast<unsigned char>(esc));
                }
                break;
            }
        }
    }
    return bytes;
}

bool extract_string_literal_initializer(
    const std::shared_ptr<ast::Initializer>& init,
    std::vector<int32_t>& outBytesWithTerminator) {
    outBytesWithTerminator.clear();
    auto exprInit = std::dynamic_pointer_cast<ExprInitializer>(init);
    if (!exprInit) return false;
    auto expr = std::dynamic_pointer_cast<Expr>(exprInit->expr);
    auto lit = std::dynamic_pointer_cast<LiteralExpr>(expr);
    if (!lit || lit->type != LiteralExpr::STRING) return false;

    outBytesWithTerminator = parse_string_literal_bytes(lit->value);
    outBytesWithTerminator.push_back(0); // C 字符串存储包含终止符
    return true;
}

Value build_zero_scalar(IRBuilder& builder, Type ty) {
    if (!ty) return builder.build_integer_const(0);
    if (is_float_type(ty)) {
        return builder.build_float_const(0.0f);
    }
    return builder.build_integer_const(0);
}

void collect_switch_labels(
    const std::shared_ptr<Stmt>& stmt,
    std::vector<std::shared_ptr<CaseStmt>>& cases,
    std::vector<std::shared_ptr<DefaultStmt>>& defaults) {
    if (!stmt) return;

    if (auto cs = std::dynamic_pointer_cast<CaseStmt>(stmt)) {
        cases.push_back(cs);
        collect_switch_labels(cs->stmt, cases, defaults);
        return;
    }
    if (auto ds = std::dynamic_pointer_cast<DefaultStmt>(stmt)) {
        defaults.push_back(ds);
        collect_switch_labels(ds->stmt, cases, defaults);
        return;
    }
    if (std::dynamic_pointer_cast<SwitchStmt>(stmt)) {
        // nested switch has its own case/default labels
        return;
    }
    if (auto comp = std::dynamic_pointer_cast<CompoundStmt>(stmt)) {
        for (const auto& item : comp->items) {
            auto stmtItem = std::dynamic_pointer_cast<StmtItem>(item);
            if (!stmtItem) continue;
            collect_switch_labels(stmtItem->stmt, cases, defaults);
        }
        return;
    }
    if (auto ls = std::dynamic_pointer_cast<LabelStmt>(stmt)) {
        collect_switch_labels(ls->stmt, cases, defaults);
        return;
    }
    if (auto ifs = std::dynamic_pointer_cast<IfStmt>(stmt)) {
        collect_switch_labels(ifs->thenStmt, cases, defaults);
        collect_switch_labels(ifs->elseStmt, cases, defaults);
        return;
    }
    if (auto ws = std::dynamic_pointer_cast<WhileStmt>(stmt)) {
        collect_switch_labels(ws->body, cases, defaults);
        return;
    }
    if (auto dws = std::dynamic_pointer_cast<DoWhileStmt>(stmt)) {
        collect_switch_labels(dws->body, cases, defaults);
        return;
    }
    if (auto fs = std::dynamic_pointer_cast<ForStmt>(stmt)) {
        collect_switch_labels(fs->body, cases, defaults);
        return;
    }
}

} // namespace

// ============================================================================
// 程序构建入口
// ============================================================================

ErrorCode IRBuilder::build_from_AST(std::shared_ptr<TranslationUnit> node) {
    hadError_ = false;
    program = std::make_shared<Program>();
    program->values.kind = SliceItemKind::VALUE;
    program->funcs.kind = SliceItemKind::FUNCTION;
    
    // 遍历所有外部声明
    for (auto& ed : node->externalDecls) {
        if (auto decl = std::dynamic_pointer_cast<Declaration>(ed)) {
            for (const auto& initDecl : decl->initDeclarators) {
                if (!initDecl || !initDecl->declarator) continue;

                Type ty = build_type(decl->specs, initDecl->declarator);
                if (ty && ty->tag == TypeTag::FUNCTION) {
                    auto func = build_function_declaration(decl, initDecl);
                    if (func) {
                        program->funcs.buffer.push_back(func);
                    }
                    continue;
                }

                auto globalDecl = std::make_shared<Declaration>();
                globalDecl->specs = decl->specs;
                globalDecl->initDeclarators.push_back(initDecl);
                auto val = build_global_declaration(globalDecl);
                if (val) {
                    program->values.buffer.push_back(val);
                }
            }
        } else if (auto fdef = std::dynamic_pointer_cast<FunctionDef>(ed)) {
            auto func = build_function(fdef);
            if (func) {
                program->funcs.buffer.push_back(func);
            }
        }
    }
    
    return hadError_ ? ErrorCode::INVALID_PROGRAM : ErrorCode::SUCCESS;
}

void IRBuilder::delete_program() {
    program.reset();
}

std::shared_ptr<Program> IRBuilder::get_program() const {
    return program;
}

// ============================================================================
// 函数定义
// ============================================================================

namespace {

void register_function_symbol(symtab::SymbolTable& symtab,
                              const std::string& name,
                              const Function& function) {
    if (name.empty()) {
        return;
    }

    auto current = symtab.currentScope();
    if (current) {
        auto existing = current->lookupLocal(name);
        if (existing && existing->kind == symtab::SymbolKind::Function) {
            existing->irFunction = function;
            return;
        }
    }

    auto sym = std::make_shared<symtab::Symbol>();
    sym->name = name;
    sym->kind = symtab::SymbolKind::Function;
    sym->type = nullptr;
    sym->irFunction = function;
    symtab.define(sym);
}

} // namespace

// TODO: fill param
Function IRBuilder::build_function(const std::shared_ptr<FunctionDef>& fdef) {
    if (!fdef) return nullptr;
    if (fdef->declarator && direct_declarator_has_varargs(fdef->declarator->direct)) {
        std::cerr << "Error: variadic function definitions are unsupported" << std::endl;
        hadError_ = true;
        return nullptr;
    }

    // 构建函数类型
    Type funcType = build_type(fdef->specs, fdef->declarator);
    if (!funcType) return nullptr;
    
    // 创建函数数据
    auto funcData = std::make_shared<FunctionData>();
    funcData->ty = funcType;
    currentFunc_ = funcData;
    bbNameCounters_.clear();
    labelBBs_.clear();
    
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
        register_function_symbol(symtab, nm, funcData);
    }

    // 进入函数体作用域
    symtab.enterScope();
    
    // 创建入口基本块
    BasicBlock entry = create_basic_block("entry");
    set_insert_point(entry);

    funcData->params.kind = SliceItemKind::VALUE;
    auto params = fdef->declarator ? extract_function_params(fdef->declarator->direct)
                                   : std::vector<std::shared_ptr<ParameterDecl>>{};
    for (std::size_t index = 0; index < params.size(); ++index) {
        const auto& param = params[index];
        if (!param || !param->specs) continue;

        auto paramDecl = std::dynamic_pointer_cast<Declarator>(param->declarator);
        Type paramTy = build_type(param->specs, paramDecl);
        if (!paramTy) continue;

        auto argRef = std::make_shared<ValueData>();
        argRef->ty = paramTy;
        argRef->name = "%arg" + std::to_string(index);
        argRef->kind.tag = ValueTag::FUNC_ARG_REF;
        argRef->kind.data = FuncArgRef{index};
        funcData->params.buffer.push_back(argRef);

        std::string paramName;
        if (paramDecl && paramDecl->direct) {
            paramName = extract_direct_decl_name(paramDecl->direct);
        }
        if (paramName.empty()) continue;

        Value addr = build_alloca(paramTy);
        addr->name = "%" + paramName;
        build_store(argRef, addr);

        auto sym = std::make_shared<symtab::Symbol>();
        sym->name = paramName;
        sym->kind = symtab::SymbolKind::Object;
        sym->type = nullptr;
        sym->irValue = addr;
        symtab.define(sym);
    }
    
    // 处理函数体
    if (auto body = std::dynamic_pointer_cast<CompoundStmt>(fdef->body)) {
        build_compound_stmt(body);
    }
    
    // 离开作用域
    symtab.leaveScope();
    
    currentFunc_ = nullptr;
    return funcData;
}

Function IRBuilder::build_function_declaration(const std::shared_ptr<Declaration>& decl,
                                               const std::shared_ptr<InitDeclarator>& initDecl) {
    if (!decl || !decl->specs || !initDecl || !initDecl->declarator) {
        return nullptr;
    }

    Type funcType = build_type(decl->specs, initDecl->declarator);
    if (!funcType || funcType->tag != TypeTag::FUNCTION) {
        return nullptr;
    }

    auto funcData = std::make_shared<FunctionData>();
    funcData->ty = funcType;
    funcData->params.kind = SliceItemKind::VALUE;
    funcData->bbs.kind = SliceItemKind::BASIC_BLOCK;

    if (initDecl->declarator->direct) {
        auto fnName = extract_direct_decl_name(initDecl->declarator->direct);
        if (!fnName.empty()) {
            funcData->name = "@" + fnName;
            register_function_symbol(symtab, fnName, funcData);
        }
    }

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

        std::vector<int32_t> strInitBytes;
        bool hasStrInit = is_char_array_type(ty) &&
                          initDecl->initializer &&
                          extract_string_literal_initializer(initDecl->initializer, strInitBytes);
        if (hasStrInit) {
            auto arrTy = std::get<std::shared_ptr<TypeKind::array>>(ty->data);
            if (arrTy && arrTy->len == 0) {
                arrTy->len = strInitBytes.size();
            }
        }
        
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
            if (hasStrInit) {
                auto arrTy = std::get<std::shared_ptr<TypeKind::array>>(ty->data);
                auto aggInit = std::make_shared<ValueData>();
                aggInit->kind.tag = ValueTag::AGGREGATE;
                Aggregate agg;
                agg.elems.kind = SliceItemKind::VALUE;

                std::size_t arrLen = arrTy ? arrTy->len : 0;
                std::size_t copied = std::min(arrLen, strInitBytes.size());
                for (std::size_t i = 0; i < copied; ++i) {
                    agg.elems.buffer.push_back(build_integer_const(strInitBytes[i]));
                }
                for (std::size_t i = copied; i < arrLen; ++i) {
                    agg.elems.buffer.push_back(build_integer_const(0));
                }
                aggInit->kind.data = agg;
                valData->kind.data = GlobalAlloc{aggInit};
            } else if (auto const_init = build_constant_initializer(initDecl->initializer, ty)) {
                valData->kind.data = GlobalAlloc{const_init};
            } else if (auto exprInit =
                           std::dynamic_pointer_cast<ExprInitializer>(initDecl->initializer)) {
                Value initVal = build_expr(std::dynamic_pointer_cast<Expr>(exprInit->expr));
                if (initVal &&
                    (initVal->kind.tag == ValueTag::NUMBER ||
                     initVal->kind.tag == ValueTag::GLOBAL_ALLOC ||
                     initVal->kind.tag == ValueTag::GET_PTR ||
                     initVal->kind.tag == ValueTag::GET_ELEM_PTR)) {
                    valData->kind.data = GlobalAlloc{initVal};
                } else {
                    valData->kind.data = GlobalAlloc{make_zero_init_value()};
                }
            } else {
                valData->kind.data = GlobalAlloc{make_zero_init_value()};
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
        std::size_t& counter = bbNameCounters_[name];
        if (counter == 0) {
            bb->name = "%" + name;
        } else {
            bb->name = "%" + name + "." + std::to_string(counter);
        }
        ++counter;
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
    } else if (auto ss = std::dynamic_pointer_cast<SwitchStmt>(stmt)) {
        build_switch_stmt(ss);
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
    } else if (auto ls = std::dynamic_pointer_cast<LabelStmt>(stmt)) {
        BasicBlock labelBB = nullptr;
        auto it = labelBBs_.find(ls->label);
        if (it == labelBBs_.end()) {
            labelBB = create_basic_block("label." + ls->label);
            labelBBs_.emplace(ls->label, labelBB);
        } else {
            labelBB = it->second;
        }

        if (!current_block_terminated()) {
            build_br(labelBB);
        }
        set_insert_point(labelBB);
        build_stmt(ls->stmt);
    } else if (auto cs = std::dynamic_pointer_cast<CaseStmt>(stmt)) {
        BasicBlock caseBB = nullptr;
        if (!switchStack_.empty()) {
            auto it = switchStack_.back().caseBBs.find(cs.get());
            if (it != switchStack_.back().caseBBs.end()) {
                caseBB = it->second;
            }
        }
        if (caseBB) {
            if (!current_block_terminated()) {
                build_br(caseBB);
            }
            set_insert_point(caseBB);
        }
        build_stmt(cs->stmt);
    } else if (auto ds = std::dynamic_pointer_cast<DefaultStmt>(stmt)) {
        BasicBlock defaultBB = nullptr;
        if (!switchStack_.empty()) {
            auto it = switchStack_.back().defaultBBs.find(ds.get());
            if (it != switchStack_.back().defaultBBs.end()) {
                defaultBB = it->second;
            }
        }
        if (defaultBB) {
            if (!current_block_terminated()) {
                build_br(defaultBB);
            }
            set_insert_point(defaultBB);
        }
        build_stmt(ds->stmt);
    } else if (auto gs = std::dynamic_pointer_cast<GotoStmt>(stmt)) {
        BasicBlock targetBB = nullptr;
        auto it = labelBBs_.find(gs->label);
        if (it == labelBBs_.end()) {
            targetBB = create_basic_block("label." + gs->label);
            labelBBs_.emplace(gs->label, targetBB);
        } else {
            targetBB = it->second;
        }
        build_br(targetBB);
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

void IRBuilder::build_switch_stmt(const std::shared_ptr<SwitchStmt>& ss) {
    if (!ss) return;

    Value cond = build_expr(std::dynamic_pointer_cast<Expr>(ss->expr));
    if (!cond) return;

    std::vector<std::shared_ptr<CaseStmt>> cases;
    std::vector<std::shared_ptr<DefaultStmt>> defaults;
    collect_switch_labels(ss->stmt, cases, defaults);

    std::vector<BasicBlock> dispatchBBs;
    const std::size_t dispatchCount = std::max<std::size_t>(1, cases.size());
    dispatchBBs.reserve(dispatchCount);
    for (std::size_t i = 0; i < dispatchCount; ++i) {
        dispatchBBs.push_back(create_basic_block("switch.dispatch"));
    }

    std::vector<BasicBlock> caseBBs;
    caseBBs.reserve(cases.size());
    for (std::size_t i = 0; i < cases.size(); ++i) {
        caseBBs.push_back(create_basic_block("switch.case"));
    }

    BasicBlock defaultBB = !defaults.empty() ? create_basic_block("switch.default") : nullptr;
    BasicBlock bodyBB = create_basic_block("switch.body");
    BasicBlock endBB = create_basic_block("switch.end");
    BasicBlock defaultTarget = defaultBB ? defaultBB : endBB;

    SwitchContext ctx;
    ctx.endBB = endBB;
    if (defaultBB) {
        ctx.defaultBBs.emplace(defaults.front().get(), defaultBB);
    }
    for (std::size_t i = 0; i < cases.size(); ++i) {
        ctx.caseBBs.emplace(cases[i].get(), caseBBs[i]);
    }

    if (!current_block_terminated()) {
        build_br(dispatchBBs.front());
    }

    // build dispatch code
    set_insert_point(dispatchBBs.front());
    if (cases.empty()) {
        build_br(defaultTarget);
    } else {
        for (std::size_t i = 0; i < cases.size(); ++i) {
            Value caseVal = build_expr(std::dynamic_pointer_cast<Expr>(cases[i]->expr));
            if (!caseVal) continue;
            Value matched = build_binary(BinaryOp::EQ, cond, caseVal);
            BasicBlock nextDispatch = (i + 1 < cases.size()) ? dispatchBBs[i + 1] : defaultTarget;
            build_cond_br(matched, caseBBs[i], nextDispatch);
            if (i + 1 < cases.size()) {
                set_insert_point(nextDispatch);
            }
        }
    }

    breakStack_.push_back(endBB);
    switchStack_.push_back(std::move(ctx));

    // Unlabeled statements before first case/default are unreachable in normal flow.
    set_insert_point(bodyBB);

    // build case
    build_stmt(ss->stmt);

    if (!current_block_terminated()) {
        build_br(endBB);
    }

    switchStack_.pop_back();
    breakStack_.pop_back();
    set_insert_point(endBB);
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
    } else if (auto cast = std::dynamic_pointer_cast<CastExpr>(expr)) {
        return build_cast_expr(cast);
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
        Value newVal = nullptr;
        if (oldVal && oldVal->ty && oldVal->ty->tag == TypeTag::POINTER &&
            rhs && rhs->ty && is_integer_type(rhs->ty) &&
            (bin->op == opcode::PLUSASSIGN || bin->op == opcode::MINUSASSIGN)) {
            Value index = rhs;
            if (bin->op == opcode::MINUSASSIGN) {
                Value zero = build_integer_const(0);
                index = build_binary(BinaryOp::SUB, zero, rhs);
            }
            newVal = build_get_ptr(oldVal, index);
        } else {
            newVal = build_binary(bop, oldVal, rhs);
        }
        build_store(newVal, addr);
        return newVal;
    }

    Value lhs = build_expr(lhsExpr);
    Value rhs = build_expr(rhsExpr);

    if (bin->op == opcode::ADD || bin->op == opcode::SUB) {
        const bool lhsPtr = lhs && lhs->ty && lhs->ty->tag == TypeTag::POINTER;
        const bool rhsPtr = rhs && rhs->ty && rhs->ty->tag == TypeTag::POINTER;
        const bool lhsInt = lhs && lhs->ty && is_integer_type(lhs->ty);
        const bool rhsInt = rhs && rhs->ty && is_integer_type(rhs->ty);

        if (bin->op == opcode::ADD) {
            if (lhsPtr && rhsInt) {
                return build_get_ptr(lhs, rhs);
            }
            if (lhsInt && rhsPtr) {
                return build_get_ptr(rhs, lhs);
            }
        } else { // SUB
            if (lhsPtr && rhsInt) {
                Value zero = build_integer_const(0);
                Value neg = build_binary(BinaryOp::SUB, zero, rhs);
                return build_get_ptr(lhs, neg);
            }
            if (lhsPtr && rhsPtr) {
                Value diff = build_binary(BinaryOp::SUB, lhs, rhs);
                if (diff) {
                    auto intTy = std::make_shared<TypeKind>();
                    intTy->tag = TypeTag::INT32;
                    diff->ty = intTy;
                }
                return diff;
            }
        }
    }

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
            Value newVal = nullptr;
            if (oldVal && oldVal->ty && oldVal->ty->tag == TypeTag::POINTER) {
                Value index = build_integer_const(un->op == opcode::PLUSPLUS ? 1 : -1);
                newVal = build_get_ptr(oldVal, index);
            } else {
                Value one = build_integer_const(1);
                newVal = build_binary(
                    un->op == opcode::PLUSPLUS ? BinaryOp::ADD : BinaryOp::SUB,
                    oldVal, one);
            }
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

        Value base = build_expr(baseExpr);
        Value idx = build_expr(idxExpr);
        if (!base || !idx) return nullptr;

        auto ptr = std::make_shared<ValueData>();
        ptr->name = "%" + std::to_string(tempCounter_++);
        Type basePointee = pointee_type(base->ty);
        if (basePointee && basePointee->tag == TypeTag::ARRAY) {
            auto arr = std::get<std::shared_ptr<TypeKind::array>>(basePointee->data);
            ptr->ty = (arr && arr->base) ? build_pointer_type(arr->base) : base->ty;
        } else {
            ptr->ty = base->ty;
        }
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
        Value newVal = nullptr;
        if (oldVal && oldVal->ty && oldVal->ty->tag == TypeTag::POINTER) {
            Value index = build_integer_const(post->op == opcode::PLUSPLUS ? 1 : -1);
            newVal = build_get_ptr(oldVal, index);
        } else {
            Value one = build_integer_const(1);
            newVal = build_binary(
                post->op == opcode::PLUSPLUS ? BinaryOp::ADD : BinaryOp::SUB,
                oldVal, one);
        }
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

    if (!callee) {
        std::cerr << "Error: undefined function: " << funcName << std::endl;
        hadError_ = true;
        return nullptr;
    }

    auto fnType = extract_function_type(callee->ty);
    if (!fnType) {
        std::cerr << "Error: callee does not have a function type" << std::endl;
        hadError_ = true;
        return nullptr;
    }

    std::vector<ir::Type> fixedParamTypes;
    fixedParamTypes.reserve(fnType->params.buffer.size());
    for (const auto& item : fnType->params.buffer) {
        if (std::holds_alternative<ir::Type>(item)) {
            fixedParamTypes.push_back(std::get<ir::Type>(item));
        }
    }

    if (args.size() < fixedParamTypes.size()) {
        std::cerr << "Error: insufficient arguments for function call: " << funcName
                  << std::endl;
        hadError_ = true;
        return nullptr;
    }
    if (!fnType->is_variadic && args.size() != fixedParamTypes.size()) {
        std::cerr << "Error: mismatched argument count for function call: " << funcName
                  << std::endl;
        hadError_ = true;
        return nullptr;
    }
    if (fnType->is_variadic) {
        for (std::size_t i = fixedParamTypes.size(); i < args.size(); ++i) {
            auto arg = args[i];
            if (!arg || !arg->ty) continue;
            if (is_float_type(arg->ty)) {
                std::cerr << "Error: floating-point variadic arguments are unsupported"
                          << std::endl;
                hadError_ = true;
                return nullptr;
            }
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

    Type objectTy = pointee_type(addr->ty);
    if (objectTy && objectTy->tag == TypeTag::ARRAY) {
        auto arrTy = std::get<std::shared_ptr<TypeKind::array>>(objectTy->data);
        if (arrTy && arrTy->base) {
            auto zero = build_integer_const(0);
            auto elemAddr = std::make_shared<ValueData>();
            elemAddr->name = "%" + std::to_string(tempCounter_++);
            elemAddr->ty = build_pointer_type(arrTy->base);
            elemAddr->kind.tag = ValueTag::GET_ELEM_PTR;
            elemAddr->kind.data = GetElemPtr{addr, zero};
            if (currentBB_) {
                currentBB_->insts.buffer.push_back(elemAddr);
            }
            return elemAddr;
        }
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

        std::vector<int32_t> strInitBytes;
        bool hasStrInit = is_char_array_type(ty) &&
                          initDecl->initializer &&
                          extract_string_literal_initializer(initDecl->initializer, strInitBytes);
        if (hasStrInit) {
            auto arrTy = std::get<std::shared_ptr<TypeKind::array>>(ty->data);
            if (arrTy && arrTy->len == 0) {
                arrTy->len = strInitBytes.size();
            }
        }

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
            if (hasStrInit) {
                auto arrTy = std::get<std::shared_ptr<TypeKind::array>>(ty->data);
                if (arrTy && arrTy->base) {
                    std::size_t copied = std::min(arrTy->len, strInitBytes.size());
                    for (std::size_t i = 0; i < copied; ++i) {
                        auto idx = build_integer_const(static_cast<int32_t>(i));
                        auto elemAddr = std::make_shared<ValueData>();
                        elemAddr->name = "%" + std::to_string(tempCounter_++);
                        elemAddr->ty = build_pointer_type(arrTy->base);
                        elemAddr->kind.tag = ValueTag::GET_ELEM_PTR;
                        elemAddr->kind.data = GetElemPtr{addr, idx};
                        if (currentBB_) currentBB_->insts.buffer.push_back(elemAddr);
                        build_store(build_integer_const(strInitBytes[i]), elemAddr);
                    }
                    for (std::size_t i = copied; i < arrTy->len; ++i) {
                        auto idx = build_integer_const(static_cast<int32_t>(i));
                        auto elemAddr = std::make_shared<ValueData>();
                        elemAddr->name = "%" + std::to_string(tempCounter_++);
                        elemAddr->ty = build_pointer_type(arrTy->base);
                        elemAddr->kind.tag = ValueTag::GET_ELEM_PTR;
                        elemAddr->kind.data = GetElemPtr{addr, idx};
                        if (currentBB_) currentBB_->insts.buffer.push_back(elemAddr);
                        build_store(build_integer_const(0), elemAddr);
                    }
                }
            } else if (auto exprInit = std::dynamic_pointer_cast<ExprInitializer>(initDecl->initializer)) {
                Value initVal = build_expr(std::dynamic_pointer_cast<Expr>(exprInit->expr));
                if (initVal) {
                    build_store(initVal, addr);
                }
            } else if (auto initList = std::dynamic_pointer_cast<InitList>(initDecl->initializer)) {
                Type objectTy = pointee_type(addr->ty);

                std::function<void(Value, Type)> emit_zero_init;
                std::function<void(Value, Type, const std::shared_ptr<InitList>&)> emit_array_init;

                emit_zero_init = [&](Value objAddr, Type objTy) {
                    if (!objAddr || !objTy) return;

                    if (objTy->tag == TypeTag::ARRAY) {
                        auto arrTy = std::get<std::shared_ptr<TypeKind::array>>(objTy->data);
                        if (!arrTy || !arrTy->base) return;

                        for (std::size_t i = 0; i < arrTy->len; ++i) {
                            auto idx = build_integer_const(static_cast<int32_t>(i));
                            auto elemAddr = std::make_shared<ValueData>();
                            elemAddr->name = "%" + std::to_string(tempCounter_++);
                            elemAddr->ty = build_pointer_type(arrTy->base);
                            elemAddr->kind.tag = ValueTag::GET_ELEM_PTR;
                            elemAddr->kind.data = GetElemPtr{objAddr, idx};
                            if (currentBB_) currentBB_->insts.buffer.push_back(elemAddr);

                            emit_zero_init(elemAddr, arrTy->base);
                        }
                        return;
                    }

                    build_store(build_zero_scalar(*this, objTy), objAddr);
                };

                emit_array_init = [&](Value arrayAddr, Type arrayTy, const std::shared_ptr<InitList>& list) {
                    if (!arrayAddr || !arrayTy || !list || arrayTy->tag != TypeTag::ARRAY) return;

                    auto arrTy = std::get<std::shared_ptr<TypeKind::array>>(arrayTy->data);
                    if (!arrTy || !arrTy->base) return;

                    const std::size_t initCount = std::min(arrTy->len, list->elements.size());
                    std::size_t i = 0;
                    for (; i < initCount; ++i) {
                        auto idx = build_integer_const(static_cast<int32_t>(i));
                        auto elemAddr = std::make_shared<ValueData>();
                        elemAddr->name = "%" + std::to_string(tempCounter_++);
                        elemAddr->ty = build_pointer_type(arrTy->base);
                        elemAddr->kind.tag = ValueTag::GET_ELEM_PTR;
                        elemAddr->kind.data = GetElemPtr{arrayAddr, idx};
                        if (currentBB_) currentBB_->insts.buffer.push_back(elemAddr);

                        auto elemInit = list->elements[i];
                        if (arrTy->base->tag == TypeTag::ARRAY) {
                            if (auto nested = std::dynamic_pointer_cast<InitList>(elemInit)) {
                                emit_array_init(elemAddr, arrTy->base, nested);
                            } else {
                                emit_zero_init(elemAddr, arrTy->base);
                            }
                        } else {
                            if (auto exprElem = std::dynamic_pointer_cast<ExprInitializer>(elemInit)) {
                                Value elemVal = build_expr(std::dynamic_pointer_cast<Expr>(exprElem->expr));
                                if (elemVal) {
                                    build_store(elemVal, elemAddr);
                                } else {
                                    build_store(build_zero_scalar(*this, arrTy->base), elemAddr);
                                }
                            } else {
                                build_store(build_zero_scalar(*this, arrTy->base), elemAddr);
                            }
                        }
                    }

                    for (; i < arrTy->len; ++i) {
                        auto idx = build_integer_const(static_cast<int32_t>(i));
                        auto elemAddr = std::make_shared<ValueData>();
                        elemAddr->name = "%" + std::to_string(tempCounter_++);
                        elemAddr->ty = build_pointer_type(arrTy->base);
                        elemAddr->kind.tag = ValueTag::GET_ELEM_PTR;
                        elemAddr->kind.data = GetElemPtr{arrayAddr, idx};
                        if (currentBB_) currentBB_->insts.buffer.push_back(elemAddr);

                        emit_zero_init(elemAddr, arrTy->base);
                    }
                };

                if (objectTy && objectTy->tag == TypeTag::ARRAY) {
                    emit_array_init(addr, objectTy, initList);
                } else if (!initList->elements.empty()) {
                    if (auto scalarInit = std::dynamic_pointer_cast<ExprInitializer>(initList->elements.front())) {
                        Value initVal = build_expr(std::dynamic_pointer_cast<Expr>(scalarInit->expr));
                        if (initVal) {
                            build_store(initVal, addr);
                        }
                    }
                }
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

Value IRBuilder::build_cast_expr(const std::shared_ptr<CastExpr>& cast) {
    if (!cast) return nullptr;

    Value src = build_expr(std::dynamic_pointer_cast<Expr>(cast->expr));
    if (!src) return nullptr;

    Type dstTy = nullptr;
    if (cast->type) {
        dstTy = build_type(cast->type->specs, nullptr);
    }
    if (!dstTy || !src->ty) {
        return src;
    }

    if (src->ty->tag == dstTy->tag) {
        return src;
    }

    if (is_integer_type(src->ty) && is_float_type(dstTy)) {
        return build_cast(CastOp::SITOFP, src, dstTy);
    }
    if (is_float_type(src->ty) && is_integer_type(dstTy)) {
        return build_cast(CastOp::FPTOSI, src, dstTy);
    }

    return src;
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

            Value base = build_expr(baseExpr);
            Value idx = build_expr(idxExpr);
            if (!base || !idx) return nullptr;

            auto valData = std::make_shared<ValueData>();
            valData->name = "%" + std::to_string(tempCounter_++);
            Type basePointee = pointee_type(base->ty);
            if (basePointee && basePointee->tag == TypeTag::ARRAY) {
                auto arr = std::get<std::shared_ptr<TypeKind::array>>(basePointee->data);
                valData->ty = (arr && arr->base) ? build_pointer_type(arr->base) : base->ty;
            } else {
                valData->ty = base->ty;
            }
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
            Type basePointee = pointee_type(base->ty);
            if (basePointee && basePointee->tag == TypeTag::ARRAY) {
                auto arr = std::get<std::shared_ptr<TypeKind::array>>(basePointee->data);
                valData->ty = (arr && arr->base) ? build_pointer_type(arr->base) : base->ty;
            } else {
                valData->ty = base->ty;
            }
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

    if (lit->type == LiteralExpr::STRING) {
        return build_string_literal_pointer(lit->value);
    }
    
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
        default:
            num.num = Integer32{0};
            valData->ty = std::make_shared<TypeKind>();
            valData->ty->tag = TypeTag::INT32;
            break;
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
    if (ptr->ty && ptr->ty->tag == TypeTag::POINTER) {
        auto ptrData = std::get<std::shared_ptr<TypeKind::pointer>>(ptr->ty->data);
        if (ptrData) {
            valData->ty = ptrData->base;
        }
    }
    
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

// for pointer calculate like ++, --
Value IRBuilder::build_get_ptr(Value src, Value index) {
    if (!src || !index) return nullptr;

    auto valData = std::make_shared<ValueData>();
    valData->name = "%" + std::to_string(tempCounter_++);
    valData->kind.tag = ValueTag::GET_PTR;
    valData->kind.data = GetPtr{src, index};
    valData->ty = src->ty;

    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
    return valData;
}

Value IRBuilder::build_binary(BinaryOp op, Value lhs, Value rhs) {
    if (!lhs || !rhs) return nullptr;
    
    auto valData = std::make_shared<ValueData>();
    valData->name = "%" + std::to_string(tempCounter_++);
    valData->kind.tag = ValueTag::BINARY;
    valData->kind.data = Binary{op, lhs, rhs};
    if (op == BinaryOp::EQ || op == BinaryOp::NOT_EQ || op == BinaryOp::LT ||
        op == BinaryOp::GT || op == BinaryOp::LE || op == BinaryOp::GE) {
        valData->ty = std::make_shared<TypeKind>();
        valData->ty->tag = TypeTag::INT32;
    } else {
        valData->ty = lhs->ty;
    }
    
    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
    
    return valData;
}

Value IRBuilder::build_cast(CastOp op, Value value, Type targetTy) {
    if (!value || !targetTy) return nullptr;

    auto valData = std::make_shared<ValueData>();
    valData->name = "%" + std::to_string(tempCounter_++);
    valData->ty = targetTy;
    valData->kind.tag = ValueTag::CAST;
    valData->kind.data = Cast{op, value};

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
    baseType->tag = TypeTag::UNIT;
    
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
        bool isVariadic = false;
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
            bool is_void_param_list = false;
            for (const auto& p : ddCall->params) {
                auto pDecl = declarator_from_parameter_ast(p ? p->declarator : nullptr);
                auto paramTy = build_type(p ? p->specs : nullptr, pDecl);
                if (ddCall->params.size() == 1 && paramTy && paramTy->tag == TypeTag::UNIT &&
                    (!p || !p->declarator)) {
                    is_void_param_list = true;
                }
                params.push_back(paramTy);
            }
            if (is_void_param_list) {
                params.clear();
            }
            TypeCtor ctor{TypeCtor::Kind::Function, 0, {}};
            ctor.params = std::move(params);
            ctor.isVariadic = ddCall->hasVarArgs;
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
                result = build_function_type(result, it->params, it->isVariadic);
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

Type IRBuilder::build_function_type(Type ret, const std::vector<Type>& params,
                                    bool isVariadic) {
    auto ty = std::make_shared<TypeKind>();
    ty->tag = TypeTag::FUNCTION;
    ty->data = std::make_shared<TypeKind::function>();
    auto funcData = std::get<std::shared_ptr<TypeKind::function>>(ty->data);
    funcData->ret = ret;
    funcData->is_variadic = isVariadic;
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
    if (func->ty && func->ty->tag == TypeTag::FUNCTION) {
        auto fnTy = std::get<std::shared_ptr<TypeKind::function>>(func->ty->data);
        if (fnTy) {
            valData->ty = fnTy->ret;
        }
    }

    if (currentBB_) {
        currentBB_->insts.buffer.push_back(valData);
    }
    return valData;
}

Value IRBuilder::build_string_literal_pointer(const std::string& raw) {
    auto it = stringLiteralGlobals_.find(raw);
    Value global = it != stringLiteralGlobals_.end() ? it->second : nullptr;

    if (!global) {
        std::vector<int32_t> bytes = parse_string_literal_bytes(raw);
        bytes.push_back(0);

        auto charTy = std::make_shared<TypeKind>();
        charTy->tag = TypeTag::INT8;
        auto arrayTy = build_array_type(charTy, bytes.size());

        auto init = std::make_shared<ValueData>();
        init->kind.tag = ValueTag::AGGREGATE;
        Aggregate agg;
        agg.elems.kind = SliceItemKind::VALUE;
        for (auto byte : bytes) {
            agg.elems.buffer.push_back(build_integer_const(byte));
        }
        init->kind.data = agg;

        global = std::make_shared<ValueData>();
        global->ty = build_pointer_type(arrayTy);
        global->name = "@.str." + std::to_string(stringLiteralCounter_++);
        global->kind.tag = ValueTag::GLOBAL_ALLOC;
        global->kind.data = GlobalAlloc{init};

        stringLiteralGlobals_.emplace(raw, global);
        if (program) {
            program->values.buffer.push_back(global);
        }
    }

    auto zero = build_integer_const(0);
    auto charTy = std::make_shared<TypeKind>();
    charTy->tag = TypeTag::INT8;
    auto ptr = std::make_shared<ValueData>();
    ptr->name = "%" + std::to_string(tempCounter_++);
    ptr->ty = build_pointer_type(charTy);
    ptr->kind.tag = ValueTag::GET_ELEM_PTR;
    ptr->kind.data = GetElemPtr{global, zero};

    if (currentBB_) {
        currentBB_->insts.buffer.push_back(ptr);
    }
    return ptr;
}

} // namespace rcc::ir
