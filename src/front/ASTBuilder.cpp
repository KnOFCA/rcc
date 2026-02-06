#include "ASTBuilder.h"
#include "AST.h"
#include "FrontSymtab.h"
#include <any>
#include <memory>
#include <algorithm>

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
        fn->body = std::any_cast<std::shared_ptr<ast::CompoundStmt>>(v);
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
    std::vector<std::shared_ptr<ast::InitDeclarator>> IDeclList(ctx->initDeclarator().size());

    std::transform(ctx->initDeclarator().begin(), ctx->initDeclarator().end(), IDeclList, [&](TParser::InitDeclaratorContext *ctxx)-> std::shared_ptr<ast::InitDeclarator>{
        return std::any_cast<std::shared_ptr<ast::InitDeclarator>>(visitInitDeclarator(ctxx));
    });

    return IDeclList;
}

std::any ASTBuilder::visitInitDeclarator(TParser::InitDeclaratorContext *ctx) {
    auto IDecl = std::make_shared<ast::InitDeclarator>();
    IDecl->declarator = std::any_cast<std::shared_ptr<ast::Declarator>>(visit(ctx->declarator()));

    //CHECK
    if(ctx->initializer())
        IDecl->initializer = std::any_cast<ast::AST>(visit(ctx->initializer()));

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

    if(ctx->parameterTypeList() || ctx->directDeclarator()){
        auto call =  std::make_shared<ast::DDCall>();
        {
            auto v = visit(ctx->directDeclarator());
            //CHECK
            call->base = std::any_cast<ast::AST>(v);
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
    auto ptr = std::make_shared<ast::Pointer>();
    ptr->qualifiers.reserve(ctx->typeQualifier().size());
    std::transform(ctx->typeQualifier().begin(), ctx->typeQualifier().end(),
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
    auto paraDecls = std::vector<std::shared_ptr<ast::ParameterDecl>>(ctx->parameterDeclaration().size());

    std::transform(ctx->parameterDeclaration().begin(), ctx->parameterDeclaration().end(), paraDecls.begin(),
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

    {
        auto v = visit(ctx->declarator());
        paraDecl->declarator = std::any_cast<ast::AST>(v);
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
    auto ele = std::vector<std::shared_ptr<ast::Initializer>>(ctx->initializer().size());
    std::transform(ctx->initializer().begin(), ctx->initializer().end(), ele.begin(),
        [&](TParser::InitializerContext *ctxx)-> std::shared_ptr<ast::Initializer>{
            return std::any_cast<std::shared_ptr<ast::Initializer>>(visitInitializer(ctxx));
        });
        
    return ele;
}

/* ============================================================ */
/* Statements                                                  */
/* ============================================================ */

std::any ASTBuilder::visitStatement(TParser::StatementContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitLabeledStatement(TParser::LabeledStatementContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitCompoundStatement(TParser::CompoundStatementContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitBlockItemList(TParser::BlockItemListContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitBlockItem(TParser::BlockItemContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitExpressionStatement(TParser::ExpressionStatementContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitSelectionStatement(TParser::SelectionStatementContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitIterationStatement(TParser::IterationStatementContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitJumpStatement(TParser::JumpStatementContext *ctx) {
    return visitChildren(ctx);
}

/* ============================================================ */
/* Expressions                                                 */
/* ============================================================ */

std::any ASTBuilder::visitExpression(TParser::ExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitAssignmentExpression(TParser::AssignmentExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitConditionalExpression(TParser::ConditionalExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitConstantExpression(TParser::ConstantExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitLogicalOrExpression(TParser::LogicalOrExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitLogicalAndExpression(TParser::LogicalAndExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitInclusiveOrExpression(TParser::InclusiveOrExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitExclusiveOrExpression(TParser::ExclusiveOrExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitAndExpression(TParser::AndExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitEqualityExpression(TParser::EqualityExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitRelationalExpression(TParser::RelationalExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitShiftExpression(TParser::ShiftExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitAdditiveExpression(TParser::AdditiveExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitMultiplicativeExpression(TParser::MultiplicativeExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitCastExpression(TParser::CastExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitUnaryExpression(TParser::UnaryExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitUnaryOperator(TParser::UnaryOperatorContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitPostfixExpression(TParser::PostfixExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitPrimaryExpression(TParser::PrimaryExpressionContext *ctx) {
    return visitChildren(ctx);
}

std::any ASTBuilder::visitArgumentExpressionList(TParser::ArgumentExpressionListContext *ctx) {
    return visitChildren(ctx);
}

/* ============================================================ */
/* Constants                                                   */
/* ============================================================ */

std::any ASTBuilder::visitConstant(TParser::ConstantContext *ctx) {
    return visitChildren(ctx);
}
