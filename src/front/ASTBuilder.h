#pragma once

#include "ANTLR/TParser.h"
#include "AST.h"
#include "./ANTLR/TParserBaseVisitor.h"

namespace rcc {

using AST = ast::AST;
using TParserBaseVisitor = antlrcpptest::TParserBaseVisitor;
using TParser = antlrcpptest::TParser;

class ASTBuilder : public TParserBaseVisitor {
public:
    AST root, now;
    ASTBuilder(): root(new ast::ASTNode), now(new ast::ASTNode) {}

    /* ============================================================
     *  Translation unit / external
     * ============================================================ */
    std::any visitTranslationUnit(TParser::TranslationUnitContext *ctx) override;
    std::any visitExternalDeclaration(TParser::ExternalDeclarationContext *ctx) override;
    std::any visitFunctionDefinition(TParser::FunctionDefinitionContext *ctx) override;
    std::any visitDeclaration(TParser::DeclarationContext *ctx) override;

    /* ============================================================
     *  Declaration specifiers
     * ============================================================ */
    std::any visitDeclarationSpecifiers(TParser::DeclarationSpecifiersContext *ctx) override;
    std::any visitDeclarationSpecifier(TParser::DeclarationSpecifierContext *ctx) override;
    std::any visitStorageClassSpecifier(TParser::StorageClassSpecifierContext *ctx) override;
    std::any visitTypeSpecifier(TParser::TypeSpecifierContext *ctx) override;
    std::any visitTypeQualifier(TParser::TypeQualifierContext *ctx) override;

    /* ============================================================
     *  Struct / Union / Enum
     * ============================================================ */
    std::any visitStructSpecifier(TParser::StructSpecifierContext *ctx) override;
    std::any visitUnionSpecifier(TParser::UnionSpecifierContext *ctx) override;
    std::any visitStructDeclarationList(TParser::StructDeclarationListContext *ctx) override;
    std::any visitStructDeclaration(TParser::StructDeclarationContext *ctx) override;
    std::any visitStructDeclaratorList(TParser::StructDeclaratorListContext *ctx) override;
    std::any visitStructDeclarator(TParser::StructDeclaratorContext *ctx) override;
    std::any visitEnumSpecifier(TParser::EnumSpecifierContext *ctx) override;
    std::any visitEnumeratorList(TParser::EnumeratorListContext *ctx) override;
    std::any visitEnumerator(TParser::EnumeratorContext *ctx) override;

    /* ============================================================
     *  Declarators / parameters
     * ============================================================ */
    std::any visitInitDeclaratorList(TParser::InitDeclaratorListContext *ctx) override;
    std::any visitInitDeclarator(TParser::InitDeclaratorContext *ctx) override;

    std::any visitDeclarator(TParser::DeclaratorContext *ctx) override;
    std::any visitDirectDeclarator(TParser::DirectDeclaratorContext *ctx) override;
    std::any visitPointer(TParser::PointerContext *ctx) override;

    std::any visitParameterTypeList(TParser::ParameterTypeListContext *ctx) override;
    std::any visitParameterList(TParser::ParameterListContext *ctx) override;
    std::any visitParameterDeclaration(TParser::ParameterDeclarationContext *ctx) override;

    /* ============================================================
     *  Initializers
     * ============================================================ */
    std::any visitInitializer(TParser::InitializerContext *ctx) override;
    std::any visitInitializerList(TParser::InitializerListContext *ctx) override;

    /* ============================================================
     *  Statements
     * ============================================================ */
    std::any visitStatement(TParser::StatementContext *ctx) override;
    std::any visitLabeledStatement(TParser::LabeledStatementContext *ctx) override;
    std::any visitCompoundStatement(TParser::CompoundStatementContext *ctx) override;
    std::any visitBlockItemList(TParser::BlockItemListContext *ctx) override;
    std::any visitBlockItem(TParser::BlockItemContext *ctx) override;
    std::any visitExpressionStatement(TParser::ExpressionStatementContext *ctx) override;
    std::any visitSelectionStatement(TParser::SelectionStatementContext *ctx) override;
    std::any visitIterationStatement(TParser::IterationStatementContext *ctx) override;
    std::any visitForInit(TParser::ForInitContext *ctx) override;
    std::any visitJumpStatement(TParser::JumpStatementContext *ctx) override;

    /* ============================================================
     *  Expressions
     * ============================================================ */
    std::any visitExpression(TParser::ExpressionContext *ctx) override;
    std::any visitAssignmentExpression(TParser::AssignmentExpressionContext *ctx) override;
    std::any visitConditionalExpression(TParser::ConditionalExpressionContext *ctx) override;
    std::any visitConstantExpression(TParser::ConstantExpressionContext *ctx) override;

    std::any visitLogicalOrExpression(TParser::LogicalOrExpressionContext *ctx) override;
    std::any visitLogicalAndExpression(TParser::LogicalAndExpressionContext *ctx) override;
    std::any visitInclusiveOrExpression(TParser::InclusiveOrExpressionContext *ctx) override;
    std::any visitExclusiveOrExpression(TParser::ExclusiveOrExpressionContext *ctx) override;
    std::any visitAndExpression(TParser::AndExpressionContext *ctx) override;
    std::any visitEqualityExpression(TParser::EqualityExpressionContext *ctx) override;
    std::any visitRelationalExpression(TParser::RelationalExpressionContext *ctx) override;
    std::any visitShiftExpression(TParser::ShiftExpressionContext *ctx) override;
    std::any visitAdditiveExpression(TParser::AdditiveExpressionContext *ctx) override;
    std::any visitMultiplicativeExpression(TParser::MultiplicativeExpressionContext *ctx) override;
    std::any visitCastExpression(TParser::CastExpressionContext *ctx) override;
    std::any visitUnaryExpression(TParser::UnaryExpressionContext *ctx) override;
    std::any visitUnaryOperator(TParser::UnaryOperatorContext *ctx) override;
    std::any visitPostfixExpression(TParser::PostfixExpressionContext *ctx) override;
    std::any visitPrimaryExpression(TParser::PrimaryExpressionContext *ctx) override;
    std::any visitArgumentExpressionList(TParser::ArgumentExpressionListContext *ctx) override;
    std::any visitTypeName(TParser::TypeNameContext *ctx) override;
    std::any visitAbstractDeclarator(TParser::AbstractDeclaratorContext *ctx) override;
    std::any visitDirectAbstractDeclarator(TParser::DirectAbstractDeclaratorContext *ctx) override;

    /* ============================================================
     *  Constants
     * ============================================================ */
    std::any visitConstant(TParser::ConstantContext *ctx) override;
};

} // namespace rcc
