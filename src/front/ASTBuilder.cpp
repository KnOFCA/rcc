#include "ASTBuilder.h"
#include "AST.h"
#include "FrontSymtab.h"
#include <any>
#include <cstddef>
#include <iostream>
#include <memory>
#include <algorithm>
#include <string>
#include <utility>
#include <functional>
#include <type_traits>
#include <concepts>

using namespace rcc;

std::any ASTBuilder::visitTranslationUnit(TParser::TranslationUnitContext *ctx) {
    auto tu = std::make_shared<ast::TranslationUnit>();

    for (auto *ext : ctx->externalDeclaration()) {

        auto v = visit(ext);

        if (!v.has_value())
            continue;

        auto node =
            std::any_cast<std::shared_ptr<ast::ExternalDecl>>(v);

        tu->externalDecls.push_back(node);
    }

    return tu;
}

std::any ASTBuilder::visitExternalDeclaration(TParser::ExternalDeclarationContext *ctx) {
    if(ctx->functionDefinition())
        return std::any_cast<std::shared_ptr<ast::ExternalDecl>>(visit(ctx->functionDefinition()));

    if(ctx->declaration())
        return std::any_cast<std::shared_ptr<ast::ExternalDecl>>(visit(ctx->declaration()));

    return nullptr;
}

std::any ASTBuilder::visitFunctionDefinition(TParser::FunctionDefinitionContext *ctx) {
    auto fn = std::make_shared<ast::FunctionDef>();

    // declarationSpecifiers
    {
        auto v = visit(ctx->declarationSpecifiers());
        fn->specs = std::any_cast<std::shared_ptr<ast::DeclSpec>>(v);
    }

    // declarator
    {
        auto v = visit(ctx->declarator());
        fn->declarator = std::any_cast<std::shared_ptr<ast::Declarator>>(v);
    }

    // compoundStatement
    {
        auto v = visit(ctx->compoundStatement());
        fn->body = std::static_pointer_cast<ast::ASTNode>(std::any_cast<std::shared_ptr<ast::Stmt>>(v));
    }

    return std::static_pointer_cast<ast::ExternalDecl>(fn);
}

std::any ASTBuilder::visitDeclaration(TParser::DeclarationContext *ctx) {
    auto decl = std::make_shared<ast::Declaration>();

    // declarationSpecifiers
    {
        auto v = visit(ctx->declarationSpecifiers());
        decl->specs = std::any_cast<std::shared_ptr<ast::DeclSpec>>(v);
    }

    // initDeclaratorList
    if (ctx->initDeclaratorList())
    {
        auto v = visit(ctx->initDeclaratorList());
        decl->initDeclarators =
            std::any_cast<std::vector<std::shared_ptr<ast::InitDeclarator>>>(v);
    }

    return std::static_pointer_cast<ast::ExternalDecl>(decl);
}

/* ============================================================ */
/* Declaration specifiers                                      */
/* ============================================================ */

std::any ASTBuilder::visitDeclarationSpecifiers(TParser::DeclarationSpecifiersContext *ctx) {
    auto node = std::make_shared<ast::DeclSpec>();

    node->specs.reserve(ctx->declarationSpecifier().size());

    for (auto *specCtx : ctx->declarationSpecifier())
    {
        auto v = visit(specCtx);

        auto spec =
            std::any_cast<std::shared_ptr<ast::DeclSpecifier>>(v);

        node->specs.push_back(spec);
    }

    return node;
}

std::any ASTBuilder::visitDeclarationSpecifier(TParser::DeclarationSpecifierContext *ctx) {
    if(ctx->storageClassSpecifier())
        return std::any_cast<std::shared_ptr<ast::DeclSpecifier>> (visit(ctx->storageClassSpecifier()));

    if(ctx->typeSpecifier())
        return std::any_cast<std::shared_ptr<ast::DeclSpecifier>> (visit(ctx->typeSpecifier()));

    if(ctx->typeQualifier())
        return std::any_cast<std::shared_ptr<ast::DeclSpecifier>> (visit(ctx->typeQualifier()));

    return nullptr;
}

std::any ASTBuilder::visitStorageClassSpecifier(TParser::StorageClassSpecifierContext *ctx) {
    auto SClassSpec = std::make_shared<ast::StorageClassSpec>();
    SClassSpec->kind = ast::DeclSpecifier::Kind::StorageClass;

    if(ctx->Auto()) SClassSpec->storageClass = ast::StorageClassSpec::Class::Auto;
    else if(ctx->Register()) SClassSpec->storageClass = ast::StorageClassSpec::Class::Register;
    else if(ctx->Static()) SClassSpec->storageClass = ast::StorageClassSpec::Class::Static;
    else if(ctx->Extern()) SClassSpec->storageClass = ast::StorageClassSpec::Class::Extern;
    else if(ctx->Typedef()) SClassSpec->storageClass = ast::StorageClassSpec::Class::Typedef;

    return SClassSpec;
}

std::any ASTBuilder::visitTypeSpecifier(TParser::TypeSpecifierContext *ctx) {
    if(ctx->structSpecifier())
        return std::static_pointer_cast<ast::DeclSpecifier>(std::any_cast<std::shared_ptr<ast::StructTypeSpec>>(visit(ctx->structSpecifier())));
    
    if(ctx->unionSpecifier())
        return std::static_pointer_cast<ast::DeclSpecifier>(std::any_cast<std::shared_ptr<ast::UnionTypeSpec>>(visit(ctx->unionSpecifier())));

    if(ctx->enumSpecifier())
        return std::static_pointer_cast<ast::DeclSpecifier>(std::any_cast<std::shared_ptr<ast::EnumTypeSpec>>(visit(ctx->enumSpecifier())));

    auto BTypeSpec = std::make_shared<ast::BuiltinTypeSpec>();
    if(ctx->Void()) BTypeSpec->builtin = ast::BuiltinTypeSpec::Builtin::Void;
    else if(ctx->Char()) BTypeSpec->builtin = ast::BuiltinTypeSpec::Builtin::Char;
    else if(ctx->Short()) BTypeSpec->builtin = ast::BuiltinTypeSpec::Builtin::Short;
    else if(ctx->Int()) BTypeSpec->builtin = ast::BuiltinTypeSpec::Builtin::Int;
    else if(ctx->Long()) BTypeSpec->builtin = ast::BuiltinTypeSpec::Builtin::Long;
    else if(ctx->Float()) BTypeSpec->builtin = ast::BuiltinTypeSpec::Builtin::Float;
    else if(ctx->Double()) BTypeSpec->builtin = ast::BuiltinTypeSpec::Builtin::Double;
    else if(ctx->Signed()) BTypeSpec->builtin = ast::BuiltinTypeSpec::Builtin::Signed;
    else if(ctx->Unsigned()) BTypeSpec->builtin = ast::BuiltinTypeSpec::Builtin::Unsigned;
    else if(ctx->Bool()) BTypeSpec->builtin = ast::BuiltinTypeSpec::Builtin::Bool;

    return std::static_pointer_cast<ast::DeclSpecifier>(BTypeSpec);
}

std::any ASTBuilder::visitTypeQualifier(TParser::TypeQualifierContext *ctx) {
    auto TypeQual = std::make_shared<ast::TypeQualifierSpec>();

    if(ctx->Const()) TypeQual->qualifier = ast::TypeQualifierSpec::Qualifier::Const;
    if(ctx->Volatile()) TypeQual->qualifier = ast::TypeQualifierSpec::Qualifier::Volatile;
    if(ctx->Restrict()) TypeQual->qualifier = ast::TypeQualifierSpec::Qualifier::Restrict;
    if(ctx->Atomic()) TypeQual->qualifier = ast::TypeQualifierSpec::Qualifier::Atomic;

    return std::static_pointer_cast<ast::DeclSpecifier>(TypeQual);
}

//TODO
/* ============================================================ */
/* Struct / Union / Enum                                       */
/* ============================================================ */

std::any ASTBuilder::visitStructSpecifier(TParser::StructSpecifierContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitUnionSpecifier(TParser::UnionSpecifierContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitStructDeclarationList(TParser::StructDeclarationListContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitStructDeclaration(TParser::StructDeclarationContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitStructDeclaratorList(TParser::StructDeclaratorListContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitStructDeclarator(TParser::StructDeclaratorContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitEnumSpecifier(TParser::EnumSpecifierContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitEnumeratorList(TParser::EnumeratorListContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitEnumerator(TParser::EnumeratorContext *ctx) {
    return visitChildren(ctx);
}

/* ============================================================ */
/* Declarators / parameters                                    */
/* ============================================================ */

std::any ASTBuilder::visitInitDeclaratorList(TParser::InitDeclaratorListContext *ctx) {
    auto inits = ctx->initDeclarator();
    std::vector<std::shared_ptr<ast::InitDeclarator>> IDeclList;
    IDeclList.reserve(inits.size());

    std::transform(inits.begin(),
        inits.end(),
        std::back_inserter(IDeclList),
        [&](TParser::InitDeclaratorContext *ctxx)-> std::shared_ptr<ast::InitDeclarator>{
            return std::any_cast<std::shared_ptr<ast::InitDeclarator>>(visitInitDeclarator(ctxx));
        });

    return IDeclList;
}

std::any ASTBuilder::visitInitDeclarator(TParser::InitDeclaratorContext *ctx) {
    auto IDecl = std::make_shared<ast::InitDeclarator>();
    IDecl->declarator = std::any_cast<std::shared_ptr<ast::Declarator>>(visit(ctx->declarator()));

    //CHECK
    if(ctx->initializer())
        IDecl->initializer = std::any_cast<std::shared_ptr<ast::Initializer>>(visit(ctx->initializer()));

    return IDecl;
}

std::any ASTBuilder::visitDeclarator(TParser::DeclaratorContext *ctx) {
    auto decl = std::make_shared<ast::Declarator>();

    if(ctx->pointer())
    {
        auto v = visit(ctx->pointer());
        decl->pointer = std::any_cast<std::shared_ptr<ast::Pointer>>(v);
    }

    {
        auto v = visit(ctx->directDeclarator());
        decl->direct = std::any_cast<std::shared_ptr<ast::DirectDeclarator>>(v);
    }
    return decl;
}

std::any ASTBuilder::visitDirectDeclarator(TParser::DirectDeclaratorContext *ctx) {
    if(ctx->Identifier()){
        auto id = std::make_shared<ast::DDIdentifier>();
        id->name = ctx->Identifier()->getText();
        return std::static_pointer_cast<ast::DirectDeclarator>(id);
    }
    
    if(ctx->declarator()){
        auto paren = std::make_shared<ast::DDParen>();
        auto v = visit(ctx->declarator());
        paren->inner = std::any_cast<ast::AST>(v);
        return std::static_pointer_cast<ast::DirectDeclarator>(paren);
    }

    if(ctx->LeftBracket()){
        auto arr = std::make_shared<ast::DDArray>();
        auto v = visit(ctx->directDeclarator());
        arr->base = std::any_cast<std::shared_ptr<ast::DirectDeclarator>>(v);
        if(ctx->constantExpression()){
            v = visit(ctx->constantExpression());
            arr->size = std::any_cast<ast::AST>(v);
        } else {
            arr->size = nullptr; // unsized array
        }
        return std::static_pointer_cast<ast::DirectDeclarator>(arr);
    }

    if(ctx->parameterTypeList() || ctx->LeftParen()){
        auto call =  std::make_shared<ast::DDCall>();
        {
            auto v = visit(ctx->directDeclarator());
            //CHECK
            call->base = std::static_pointer_cast<ast::ASTNode>(std::any_cast<std::shared_ptr<ast::DirectDeclarator>>(v));
        }
        if(ctx->parameterTypeList()){
            auto v = visit(ctx->parameterTypeList());
            call->params = std::any_cast<std::vector<std::shared_ptr<ast::ParameterDecl>>>(v);
        }
        return std::static_pointer_cast<ast::DirectDeclarator>(call);
    }

    return nullptr;
}

std::any ASTBuilder::visitPointer(TParser::PointerContext *ctx) {
    auto typeQuals = ctx->typeQualifier();
    auto ptr = std::make_shared<ast::Pointer>();
    ptr->qualifiers.reserve(typeQuals.size());
    std::transform(typeQuals.begin(), 
        typeQuals.end(),
        std::back_inserter(ptr->qualifiers),
        [&](TParser::TypeQualifierContext *ctxx)-> std::shared_ptr<ast::TypeQualifierSpec>{
            //CHECK: any_cast
            return std::any_cast<std::shared_ptr<ast::TypeQualifierSpec>>(visitTypeQualifier(ctxx));
        });
    if(ctx->pointer()){
        auto v = visit(ctx->pointer());
        ptr->next = std::any_cast<std::shared_ptr<ast::Pointer>>(v);
    }
    return ptr;
}

std::any ASTBuilder::visitParameterTypeList(TParser::ParameterTypeListContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitParameterList(TParser::ParameterListContext *ctx) {
    auto params = ctx->parameterDeclaration();
    auto paraDecls = std::vector<std::shared_ptr<ast::ParameterDecl>>(params.size());

    std::transform(params.begin(),
        params.end(),
        paraDecls.begin(),
        [&](TParser::ParameterDeclarationContext *ctxx)-> std::shared_ptr<ast::ParameterDecl>{
            return std::any_cast<std::shared_ptr<ast::ParameterDecl>>(visitParameterDeclaration(ctxx));
        });
    
    return paraDecls;
}

std::any ASTBuilder::visitParameterDeclaration(TParser::ParameterDeclarationContext *ctx) {
    auto paraDecl = std::make_shared<ast::ParameterDecl>();

    {
        auto v = visit(ctx->declarationSpecifiers());
        paraDecl->specs = std::any_cast<std::shared_ptr<ast::DeclSpec>>(v);
    }

    if(ctx->declarator())
    {
        auto v = visit(ctx->declarator());
        paraDecl->declarator = std::static_pointer_cast<ast::ASTNode>(std::any_cast<std::shared_ptr<ast::Declarator>>(v));
    }

    return paraDecl;
}

/* ============================================================ */
/* Initializers                                                */
/* ============================================================ */

// return ExprInitializer or InitList
std::any ASTBuilder::visitInitializer(TParser::InitializerContext *ctx) {
    if(ctx->assignmentExpression()){
        auto exprInit = std::make_shared<ast::ExprInitializer>();
        auto v = visit(ctx->assignmentExpression());
        exprInit->expr = std::any_cast<ast::AST>(v);
        return std::static_pointer_cast<ast::Initializer>(exprInit);
    }
    if(ctx->initializerList()){
        auto initList = std::make_shared<ast::InitList>();
        auto v = visit(ctx->initializerList());
        initList->elements = std::any_cast<std::vector<std::shared_ptr<ast::Initializer>>>(v);
        return std::static_pointer_cast<ast::Initializer>(initList);
    }
    return nullptr;
}

std::any ASTBuilder::visitInitializerList(TParser::InitializerListContext *ctx) {
    auto inits = ctx->initializer();
    auto ele = std::vector<std::shared_ptr<ast::Initializer>>(inits.size());
    std::transform(inits.begin(), 
        inits.end(), 
        ele.begin(),
        [&](TParser::InitializerContext *ctxx)-> std::shared_ptr<ast::Initializer>{
            return std::any_cast<std::shared_ptr<ast::Initializer>>(visitInitializer(ctxx));
        });

    return ele;
}

/* ============================================================ */
/* Statements                                                  */
/* ============================================================ */

//CHECK: return type
std::any ASTBuilder::visitStatement(TParser::StatementContext *ctx) {
    if(ctx->labeledStatement())
        return std::any_cast<std::shared_ptr<ast::Stmt>>(visit(ctx->labeledStatement()));
    if(ctx->compoundStatement())
        return std::any_cast<std::shared_ptr<ast::Stmt>>(visit(ctx->compoundStatement()));
    if(ctx->expressionStatement())
        return std::any_cast<std::shared_ptr<ast::Stmt>>(visit(ctx->expressionStatement()));
    if(ctx->selectionStatement())
        return std::any_cast<std::shared_ptr<ast::Stmt>>(visit(ctx->selectionStatement()));
    if(ctx->iterationStatement())    
        return std::any_cast<std::shared_ptr<ast::Stmt>>(visit(ctx->iterationStatement()));
    if(ctx->jumpStatement())
        return std::any_cast<std::shared_ptr<ast::Stmt>>(visit(ctx->jumpStatement()));
    return nullptr;
}

std::any ASTBuilder::visitLabeledStatement(TParser::LabeledStatementContext *ctx) {
    if(ctx->Identifier()) {
        auto labelStmt = std::make_shared<ast::LabelStmt>();
        labelStmt->label = ctx->Identifier()->getText();
        auto v = visit(ctx->statement());
        labelStmt->stmt = std::any_cast<std::shared_ptr<ast::Stmt>>(v);
        return std::static_pointer_cast<ast::Stmt>(labelStmt);
    }
    if(ctx->Case()) {
        auto caseStmt = std::make_shared<ast::CaseStmt>();
        auto v = visit(ctx->constantExpression());
        caseStmt->expr = std::any_cast<ast::AST>(v);
        v = visit(ctx->statement());
        caseStmt->stmt = std::any_cast<std::shared_ptr<ast::Stmt>>(v);
        return std::static_pointer_cast<ast::Stmt>(caseStmt);
    }
    if(ctx->Default()) {
        auto defaultStmt = std::make_shared<ast::DefaultStmt>();
        auto v = visit(ctx->statement());
        defaultStmt->stmt = std::any_cast<std::shared_ptr<ast::Stmt>>(v);
        return std::static_pointer_cast<ast::Stmt>(defaultStmt);
    }
    return nullptr;
}

std::any ASTBuilder::visitCompoundStatement(TParser::CompoundStatementContext *ctx) {
    auto compStmt = std::make_shared<ast::CompoundStmt>();
    //CHECK: move usage
    if(ctx->blockItemList())
        compStmt->items = std::move(std::any_cast<std::vector<std::shared_ptr<ast::BlockItem>>>(visitBlockItemList(ctx->blockItemList())));
    return std::static_pointer_cast<ast::Stmt>(compStmt);
}

std::any ASTBuilder::visitBlockItemList(TParser::BlockItemListContext *ctx) {
    auto blockItems = ctx->blockItem();
    auto items = std::vector<std::shared_ptr<ast::BlockItem>>(blockItems.size());
    std::transform(blockItems.begin(),
    blockItems.end(),
    std::back_inserter(items),
    [&](TParser::BlockItemContext *ctxx)-> std::shared_ptr<ast::BlockItem>{
        return std::any_cast<std::shared_ptr<ast::BlockItem>>(visitBlockItem(ctxx));
    });
    return items;
}

std::any ASTBuilder::visitBlockItem(TParser::BlockItemContext *ctx) {
    if(ctx->declaration()){
        return std::any_cast<std::shared_ptr<ast::BlockItem>>(visit(ctx->declaration()));
    }
    if(ctx->statement()){
        return std::any_cast<std::shared_ptr<ast::BlockItem>>(visit(ctx->statement()));
    }
    return nullptr;
}

std::any ASTBuilder::visitExpressionStatement(TParser::ExpressionStatementContext *ctx) {
    auto exprStmt = std::make_shared<ast::ExprStmt>();
    if(ctx->expression()){
        auto v = visit(ctx->expression());
        exprStmt->expr = std::any_cast<ast::AST>(v);
    } else {
        exprStmt->expr = nullptr;
    }
    return std::static_pointer_cast<ast::Stmt>(exprStmt);
}

std::any ASTBuilder::visitSelectionStatement(TParser::SelectionStatementContext *ctx) {
    if(ctx->If()){
        auto ifStmt = std::make_shared<ast::IfStmt>();
        auto v = visit(ctx->expression());
        ifStmt->cond = std::any_cast<ast::AST>(v);
        v = visit(ctx->statement(0));
        ifStmt->thenStmt = std::any_cast<std::shared_ptr<ast::Stmt>>(v);
        if(ctx->Else()){
            v = visit(ctx->statement(1));
            ifStmt->elseStmt = std::any_cast<std::shared_ptr<ast::Stmt>>(v);
        }
        return std::static_pointer_cast<ast::Stmt>(ifStmt);
    }
    if(ctx->Switch()){
        auto switchStmt = std::make_shared<ast::SwitchStmt>();
        auto v = visit(ctx->expression());
        switchStmt->expr = std::any_cast<ast::AST>(v);
        //CHECK: statement(0) exists
        v = visit(ctx->statement(0));
        switchStmt->stmt = std::any_cast<std::shared_ptr<ast::Stmt>>(v);
        return std::static_pointer_cast<ast::Stmt>(switchStmt);
    }
    return nullptr;
}

std::any ASTBuilder::visitIterationStatement(TParser::IterationStatementContext *ctx) {
    if(ctx->While()){
        if(ctx->Do()){
            auto doWhileStmt = std::make_shared<ast::DoWhileStmt>();
            auto v = visit(ctx->statement());
            doWhileStmt->body = std::any_cast<std::shared_ptr<ast::Stmt>>(v);
            v = visit(ctx->expression());
            doWhileStmt->cond = std::any_cast<ast::AST>(v);
            return std::static_pointer_cast<ast::Stmt>(doWhileStmt);
        } else {
            auto whileStmt = std::make_shared<ast::WhileStmt>();
            auto v = visit(ctx->expression());
            whileStmt->cond = std::any_cast<ast::AST>(v);
            v = visit(ctx->statement());
            whileStmt->body = std::any_cast<std::shared_ptr<ast::Stmt>>(v);
            return std::static_pointer_cast<ast::Stmt>(whileStmt);
        }
    }
    if(ctx->For()){
        auto forStmt = std::make_shared<ast::ForStmt>();
        if(ctx->forInit()){
            auto v = visit(ctx->forInit());
            forStmt->init = std::any_cast<ast::AST>(v);
        } else {
            forStmt->init = nullptr;
        }
        if(ctx->expressionStatement()){
            auto v = visit(ctx->expressionStatement());
            forStmt->cond = std::any_cast<std::shared_ptr<ast::Stmt>>(v);
        } else {
            forStmt->cond = nullptr;
        }
        if(ctx->expression()){
            auto v = visit(ctx->expression());
            forStmt->iter = std::any_cast<ast::AST>(v);
        } else {
            forStmt->iter = nullptr;
        }
        auto v = visit(ctx->statement());
        forStmt->body = std::any_cast<std::shared_ptr<ast::Stmt>>(v);
        return std::static_pointer_cast<ast::Stmt>(forStmt);
    }
    return nullptr;
}

std::any ASTBuilder::visitForInit(TParser::ForInitContext *ctx) {
    if(ctx->expressionStatement()){
        auto v = visit(ctx->expressionStatement());
        return std::any_cast<ast::AST>(v);
    }
    if(ctx->declaration()){
        auto v = visit(ctx->declaration());
        return std::any_cast<ast::AST>(v);
    }
    return nullptr;
}

std::any ASTBuilder::visitJumpStatement(TParser::JumpStatementContext *ctx) {
    if(ctx->Goto()){
        auto gotoStmt = std::make_shared<ast::GotoStmt>();
        gotoStmt->label = ctx->Identifier()->getText();
        return std::static_pointer_cast<ast::Stmt>(gotoStmt);
    }
    if(ctx->Continue()){
        auto continueStmt = std::make_shared<ast::ContinueStmt>();
        return std::static_pointer_cast<ast::Stmt>(continueStmt);
    }
    if(ctx->Break()){
        auto breakStmt = std::make_shared<ast::BreakStmt>();
        return std::static_pointer_cast<ast::Stmt>(breakStmt);
    }
    if(ctx->Return()){
        auto returnStmt = std::make_shared<ast::ReturnStmt>();
        if(ctx->expression()){
            auto v = visit(ctx->expression());
            returnStmt->expr = std::any_cast<ast::AST>(v);
        } else {    
            returnStmt->expr = nullptr;
        }
        return std::static_pointer_cast<ast::Stmt>(returnStmt);
    }
    return nullptr;
}

/* ============================================================ */
/* Expressions                                                 */
/* ============================================================ */

// constexpr char commaOp[] = ",";
// constexpr char assignOp[] = "=";
// constexpr char ororOp[] = "||";
// constexpr char andandOp[] = "&&";
// constexpr char bitorOp[] = "|";
// constexpr char bitxorOp[] = "^";
// constexpr char bitandOp[] = "&";
// constexpr char eqOp[] = "==";
// constexpr char neqOp[] = "!=";
// constexpr char ltOp[] = "<";
// constexpr char gtOp[] = ">";
// constexpr char leOp[] = "<=";
// constexpr char geOp[] = ">=";
// constexpr char addOp[] = "+";
// constexpr char subOp[] = "-";
// constexpr char mulOp[] = "*";
// constexpr char divOp[] = "/";
// constexpr char modOp[] = "%";

template<typename T>
concept IsParserRuleContextPtr = 
    std::is_pointer_v<std::remove_reference_t<T>> &&
    std::is_base_of_v<
        antlr4::ParserRuleContext, 
        std::remove_pointer_t<std::remove_reference_t<T>>
    >;

template <IsParserRuleContextPtr T, ast::opcode Op>
ast::AST buildBiExpr(ASTBuilder* builder, const std::vector<T>& expr, size_t index = 0) {
    size_t count = expr.size();

    if(count == 0) {
        return nullptr;
    }

    if (index == count - 1) {
        auto v = builder->visit(expr[index]);
        return std::any_cast<ast::AST>(v);
    } else {
        auto leftExpr = std::make_shared<ast::BinaryExpr>();
        leftExpr->op = Op;
        auto v = builder->visit(expr[index]);
        leftExpr->lhs = std::any_cast<ast::AST>(v);
        leftExpr->rhs = buildBiExpr<T, Op>(builder, expr, index + 1);
        return leftExpr;
    }
}

std::any ASTBuilder::visitExpression(TParser::ExpressionContext *ctx) {
    
    return buildBiExpr<decltype(ctx->assignmentExpression())::value_type, ast::opcode::COMMA>(this, ctx->assignmentExpression());
}

std::any ASTBuilder::visitAssignmentExpression(TParser::AssignmentExpressionContext *ctx) {
    if(ctx->Assign()){
        auto assignExpr = std::make_shared<ast::BinaryExpr>();
        assignExpr->op = ast::opcode::ASSIGN;
        auto v = visit(ctx->unaryExpression());
        assignExpr->lhs = std::any_cast<ast::AST>(v);
        v = visit(ctx->assignmentExpression());
        assignExpr->rhs = std::any_cast<ast::AST>(v);
        return std::static_pointer_cast<ast::ASTNode>(assignExpr);
    } else {
        return visitChildren(ctx);
    }
}

std::any ASTBuilder::visitConditionalExpression(TParser::ConditionalExpressionContext *ctx) {
    auto condExpr = std::make_shared<ast::ConditionalExpr>();

    auto v = visit(ctx->logicalOrExpression());
    condExpr->cond = std::any_cast<ast::AST>(v);

    if(ctx->Question()){
        v = visit(ctx->expression());
        condExpr->thenExpr = std::any_cast<ast::AST>(v);
        v = visit(ctx->conditionalExpression());
        condExpr->elseExpr = std::any_cast<ast::AST>(v);
    } else {
        condExpr->thenExpr = condExpr->cond; //CHECK: fill with cond or not
        condExpr->elseExpr = nullptr;
    }

    return std::static_pointer_cast<ast::ASTNode>(condExpr);
}

//NOTE: pass through all expressions like normal conditional expression, but do check during semantic analysis to ensure the correct precedence and associativity
std::any ASTBuilder::visitConstantExpression(TParser::ConstantExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitLogicalOrExpression(TParser::LogicalOrExpressionContext *ctx) {
    return buildBiExpr<decltype(ctx->logicalAndExpression())::value_type, ast::opcode::OROR>(this, ctx->logicalAndExpression());
}

std::any ASTBuilder::visitLogicalAndExpression(TParser::LogicalAndExpressionContext *ctx) {
    return buildBiExpr<decltype(ctx->inclusiveOrExpression())::value_type, ast::opcode::ANDAND>(this, ctx->inclusiveOrExpression());
}

std::any ASTBuilder::visitInclusiveOrExpression(TParser::InclusiveOrExpressionContext *ctx) {
    return buildBiExpr<decltype(ctx->exclusiveOrExpression())::value_type, ast::opcode::BITOR>(this, ctx->exclusiveOrExpression());
}

std::any ASTBuilder::visitExclusiveOrExpression(TParser::ExclusiveOrExpressionContext *ctx) {
    return buildBiExpr<decltype(ctx->andExpression())::value_type, ast::opcode::BITXOR>(this, ctx->andExpression());
}

std::any ASTBuilder::visitAndExpression(TParser::AndExpressionContext *ctx) {
    return buildBiExpr<decltype(ctx->equalityExpression())::value_type, ast::opcode::BITAND>(this, ctx->equalityExpression());
}

std::any ASTBuilder::visitEqualityExpression(TParser::EqualityExpressionContext *ctx) {
    auto exprs = ctx->relationalExpression();
    
    if (exprs.empty()) {
        return nullptr;
    }
    
    std::vector<ast::opcode> ops;
    for (auto child : ctx->children) {
        if (antlr4::tree::TerminalNode* node = 
            dynamic_cast<antlr4::tree::TerminalNode*>(child)) {
            if (node->getSymbol()->getType() == TParser::Equal) {
                ops.push_back(ast::opcode::EQ);
            }
            else if (node->getSymbol()->getType() == TParser::NotEqual) {
                ops.push_back(ast::opcode::NEQ);
            }
        }
    }
    
    auto result = visit(exprs[0]);
    ast::AST left = std::any_cast<ast::AST>(result);
    
    for (size_t i = 0; i < ops.size(); ++i) {
        auto binaryExpr = std::make_shared<ast::BinaryExpr>();
        binaryExpr->op = ops[i];
        binaryExpr->lhs = left;
        
        auto rightResult = visit(exprs[i + 1]);
        binaryExpr->rhs = std::any_cast<ast::AST>(rightResult);
        
        left = std::static_pointer_cast<ast::ASTNode>(binaryExpr);
    }
    
    return left;
}

std::any ASTBuilder::visitRelationalExpression(TParser::RelationalExpressionContext *ctx) {
        auto exprs = ctx->shiftExpression();
    
    if (exprs.empty()) {
        return nullptr;
    }
    
    std::vector<ast::opcode> ops;
    for (auto child : ctx->children) {
        if (antlr4::tree::TerminalNode* node = 
            dynamic_cast<antlr4::tree::TerminalNode*>(child)) {
            switch(node->getSymbol()->getType()) {
                case TParser::Less:
                    ops.push_back(ast::opcode::LT);
                    break;
                case TParser::Greater:
                    ops.push_back(ast::opcode::GT);
                    break;
                case TParser::LessEqual:
                    ops.push_back(ast::opcode::LE);
                    break;
                case TParser::GreaterEqual:
                    ops.push_back(ast::opcode::GE);
                    break;
                default:
                    break;
            }
        }
    }
    
    auto result = visit(exprs[0]);
    ast::AST left = std::any_cast<ast::AST>(result);
    
    for (size_t i = 0; i < ops.size(); ++i) {
        auto binaryExpr = std::make_shared<ast::BinaryExpr>();
        binaryExpr->op = ops[i];
        binaryExpr->lhs = left;
        
        auto rightResult = visit(exprs[i + 1]);
        binaryExpr->rhs = std::any_cast<ast::AST>(rightResult);
        
        left = std::static_pointer_cast<ast::ASTNode>(binaryExpr);
    }
    
    return left;
}

//TODO: add shiftexpr grammar
std::any ASTBuilder::visitShiftExpression(TParser::ShiftExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitAdditiveExpression(TParser::AdditiveExpressionContext *ctx) {
    auto exprs = ctx->multiplicativeExpression();
    
    if (exprs.empty()) {
        return nullptr;
    }
    
    std::vector<ast::opcode> ops;
    for (auto child : ctx->children) {
        if (antlr4::tree::TerminalNode* node = 
            dynamic_cast<antlr4::tree::TerminalNode*>(child)) {
            if (node->getSymbol()->getType() == TParser::Plus) {
                ops.push_back(ast::opcode::ADD);
            }
            else if (node->getSymbol()->getType() == TParser::Minus) {
                ops.push_back(ast::opcode::SUB);
            }
        }
    }
    
    auto result = visit(exprs[0]);
    ast::AST left = std::any_cast<ast::AST>(result);
    
    for (size_t i = 0; i < ops.size(); ++i) {
        auto binaryExpr = std::make_shared<ast::BinaryExpr>();
        binaryExpr->op = ops[i];
        binaryExpr->lhs = left;
        
        auto rightResult = visit(exprs[i + 1]);
        binaryExpr->rhs = std::any_cast<ast::AST>(rightResult);
        
        left = std::static_pointer_cast<ast::ASTNode>(binaryExpr);
    }
    
    return left;
}

std::any ASTBuilder::visitMultiplicativeExpression(TParser::MultiplicativeExpressionContext *ctx) {
    auto exprs = ctx->castExpression();
    
    if (exprs.empty()) {
        return nullptr;
    }
    
    std::vector<ast::opcode> ops;
    for (auto child : ctx->children) {
        if (antlr4::tree::TerminalNode* node = 
            dynamic_cast<antlr4::tree::TerminalNode*>(child)) {
            switch(node->getSymbol()->getType()) {
                case TParser::Star:
                    ops.push_back(ast::opcode::MUL);
                    break;
                case TParser::Slash:
                    ops.push_back(ast::opcode::DIV);
                    break;
                case TParser::Percent:
                    ops.push_back(ast::opcode::MOD);
                    break;
                default:
                    break;
            }
        }
    }
    
    auto result = visit(exprs[0]);
    ast::AST left = std::any_cast<ast::AST>(result);
    
    for (size_t i = 0; i < ops.size(); ++i) {
        auto binaryExpr = std::make_shared<ast::BinaryExpr>();
        binaryExpr->op = ops[i];
        binaryExpr->lhs = left;
        
        auto rightResult = visit(exprs[i + 1]);
        binaryExpr->rhs = std::any_cast<ast::AST>(rightResult);
        
        left = std::static_pointer_cast<ast::ASTNode>(binaryExpr);
    }
    
    return left;
}

//TODO: add castexpr grammar
std::any ASTBuilder::visitCastExpression(TParser::CastExpressionContext *ctx) {
    return visitChildren(ctx);
}

//CHECK: AI implementation
std::any ASTBuilder::visitUnaryExpression(TParser::UnaryExpressionContext *ctx) {
    // ++ unaryExpression | -- unaryExpression
    if (ctx->PlusPlus()) {
        auto ue = std::make_shared<ast::UnaryExpr>();
        ue->op = ast::opcode::PLUSPLUS;
        auto v = visit(ctx->unaryExpression());
        ue->operand = std::any_cast<ast::AST>(v);
        return std::static_pointer_cast<ast::ASTNode>(ue);
    }
    if (ctx->MinusMinus()) {
        auto ue = std::make_shared<ast::UnaryExpr>();
        ue->op = ast::opcode::MINUSMINUS;
        auto v = visit(ctx->unaryExpression());
        ue->operand = std::any_cast<ast::AST>(v);
        return std::static_pointer_cast<ast::ASTNode>(ue);
    }

    // unaryOperator castExpression
    if (ctx->unaryOperator()) {
        auto anyop = visit(ctx->unaryOperator());
        ast::opcode op = std::any_cast<ast::opcode>(anyop);
        auto ue = std::make_shared<ast::UnaryExpr>();
        ue->op = op;
        auto v = visit(ctx->castExpression());
        ue->operand = std::any_cast<ast::AST>(v);
        return std::static_pointer_cast<ast::ASTNode>(ue);
    }

    // postfixExpression
    if (ctx->postfixExpression()) {
        return visit(ctx->postfixExpression());
    }

    return nullptr;
}

std::any ASTBuilder::visitUnaryOperator(TParser::UnaryOperatorContext *ctx) {
    // Map tokens to opcode where possible
    if (ctx->And()) {
        return ast::opcode::AMP;
    }
    if (ctx->Star()) {
        return ast::opcode::STAR;
    }
    if (ctx->Plus()) {
        return ast::opcode::ADD; // unary plus represented with ADD
    }
    if (ctx->Minus()) {
        return ast::opcode::SUB; // unary minus represented with SUB
    }
    // Tilde/Not not represented explicitly in opcode enum -> NONE
    return ast::opcode::NONE;
}

std::any ASTBuilder::visitPostfixExpression(TParser::PostfixExpressionContext *ctx) {
    // Start from primaryExpression
    auto v = visit(ctx->primaryExpression());
    ast::AST expr = std::any_cast<ast::AST>(v);

    // Iterate children after primaryExpression to handle calls and postfix ++/--
    auto children = ctx->children;
    if (children.size() <= 1) return expr;

    for (size_t i = 1; i < children.size(); ) {
        auto child = children[i];

        if (antlr4::tree::TerminalNode* tn = dynamic_cast<antlr4::tree::TerminalNode*>(child)) {
            int tt = tn->getSymbol()->getType();
            if (tt == TParser::PlusPlus) {
                auto p = std::make_shared<ast::PostfixExpr>();
                p->op = ast::opcode::PLUSPLUS;
                p->expr = expr;
                expr = std::static_pointer_cast<ast::ASTNode>(p);
                ++i;
                continue;
            }
            if (tt == TParser::MinusMinus) {
                auto p = std::make_shared<ast::PostfixExpr>();
                p->op = ast::opcode::MINUSMINUS;
                p->expr = expr;
                expr = std::static_pointer_cast<ast::ASTNode>(p);
                ++i;
                continue;
            }
            if (tt == TParser::LeftParen) {
                // Look ahead for optional argumentExpressionList then RightParen
                std::vector<ast::AST> args;
                if (i + 1 < children.size()) {
                    if (auto argCtx = dynamic_cast<TParser::ArgumentExpressionListContext*>(children[i+1])) {
                        auto anyArgs = visitArgumentExpressionList(argCtx);
                        args = std::any_cast<std::vector<ast::AST>>(anyArgs);
                        // skip LeftParen, argList, RightParen
                        i += 3; // LeftParen, argList, RightParen
                    } else {
                        // no args: skip LeftParen and RightParen
                        i += 2;
                    }
                } else {
                    // malformed but skip
                    ++i;
                }

                auto call = std::make_shared<ast::CallExpr>();
                call->callee = expr;
                call->args = std::move(args);
                expr = std::static_pointer_cast<ast::ASTNode>(call);
                continue;
            }
            // other terminals (RightParen etc.) -- skip
            ++i;
            continue;
        }

        // If child is an argumentExpressionList (should be handled in LeftParen branch), skip it
        if (dynamic_cast<TParser::ArgumentExpressionListContext*>(child)) {
            ++i;
            continue;
        }

        ++i;
    }

    return expr;
}

std::any ASTBuilder::visitPrimaryExpression(TParser::PrimaryExpressionContext *ctx) {
    if (ctx->Identifier()) {
        auto id = std::make_shared<ast::IdExpr>();
        id->name = ctx->Identifier()->getText();
        return std::static_pointer_cast<ast::ASTNode>(id);
    }
    if (ctx->constant()) {
        return visit(ctx->constant());
    }
    if (ctx->StringLiteral()) {
        auto lit = std::make_shared<ast::LiteralExpr>();
        lit->value = ctx->StringLiteral()->getText();
        return std::static_pointer_cast<ast::ASTNode>(lit);
    }
    if (ctx->expression()) {
        return visit(ctx->expression());
    }
    return nullptr;
}

std::any ASTBuilder::visitArgumentExpressionList(TParser::ArgumentExpressionListContext *ctx) {
    std::vector<ast::AST> args(ctx->assignmentExpression().size());
    for (size_t i = 0; i < ctx->assignmentExpression().size(); ++i) {
        auto v = visit(ctx->assignmentExpression()[i]);
        args[i] = std::any_cast<ast::AST>(v);
    }
    return args;
}

/* ============================================================ */
/* Constants                                                   */
/* ============================================================ */

std::any ASTBuilder::visitConstant(TParser::ConstantContext *ctx) {
    if (ctx->IntegerConstant()) {
        auto lit = std::make_shared<ast::LiteralExpr>();
        lit->value = ctx->IntegerConstant()->getText();
        return std::static_pointer_cast<ast::ASTNode>(lit);
    }
    if (ctx->FloatingConstant()) {
        auto lit = std::make_shared<ast::LiteralExpr>();
        lit->value = ctx->FloatingConstant()->getText();
        return std::static_pointer_cast<ast::ASTNode>(lit);
    }
    if (ctx->CharacterConstant()) {
        auto lit = std::make_shared<ast::LiteralExpr>();
        lit->value = ctx->CharacterConstant()->getText();
        return std::static_pointer_cast<ast::ASTNode>(lit);
    }

    return nullptr;
}
