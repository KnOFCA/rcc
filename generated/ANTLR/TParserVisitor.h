
// Generated from ANTLR/TParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "TParser.h"


namespace antlrcpptest {

/**
 * This class defines an abstract visitor for a parse tree
 * produced by TParser.
 */
class  TParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by TParser.
   */
    virtual std::any visitTranslationUnit(TParser::TranslationUnitContext *context) = 0;

    virtual std::any visitExternalDeclaration(TParser::ExternalDeclarationContext *context) = 0;

    virtual std::any visitFunctionDefinition(TParser::FunctionDefinitionContext *context) = 0;

    virtual std::any visitDeclaration(TParser::DeclarationContext *context) = 0;

    virtual std::any visitDeclarationSpecifiers(TParser::DeclarationSpecifiersContext *context) = 0;

    virtual std::any visitDeclarationSpecifier(TParser::DeclarationSpecifierContext *context) = 0;

    virtual std::any visitStorageClassSpecifier(TParser::StorageClassSpecifierContext *context) = 0;

    virtual std::any visitTypeSpecifier(TParser::TypeSpecifierContext *context) = 0;

    virtual std::any visitTypeQualifier(TParser::TypeQualifierContext *context) = 0;

    virtual std::any visitStructSpecifier(TParser::StructSpecifierContext *context) = 0;

    virtual std::any visitUnionSpecifier(TParser::UnionSpecifierContext *context) = 0;

    virtual std::any visitStructDeclarationList(TParser::StructDeclarationListContext *context) = 0;

    virtual std::any visitStructDeclaration(TParser::StructDeclarationContext *context) = 0;

    virtual std::any visitStructDeclaratorList(TParser::StructDeclaratorListContext *context) = 0;

    virtual std::any visitStructDeclarator(TParser::StructDeclaratorContext *context) = 0;

    virtual std::any visitEnumSpecifier(TParser::EnumSpecifierContext *context) = 0;

    virtual std::any visitEnumeratorList(TParser::EnumeratorListContext *context) = 0;

    virtual std::any visitEnumerator(TParser::EnumeratorContext *context) = 0;

    virtual std::any visitInitDeclaratorList(TParser::InitDeclaratorListContext *context) = 0;

    virtual std::any visitInitDeclarator(TParser::InitDeclaratorContext *context) = 0;

    virtual std::any visitDeclarator(TParser::DeclaratorContext *context) = 0;

    virtual std::any visitDirectDeclarator(TParser::DirectDeclaratorContext *context) = 0;

    virtual std::any visitPointer(TParser::PointerContext *context) = 0;

    virtual std::any visitParameterTypeList(TParser::ParameterTypeListContext *context) = 0;

    virtual std::any visitParameterList(TParser::ParameterListContext *context) = 0;

    virtual std::any visitParameterDeclaration(TParser::ParameterDeclarationContext *context) = 0;

    virtual std::any visitInitializer(TParser::InitializerContext *context) = 0;

    virtual std::any visitInitializerList(TParser::InitializerListContext *context) = 0;

    virtual std::any visitStatement(TParser::StatementContext *context) = 0;

    virtual std::any visitLabeledStatement(TParser::LabeledStatementContext *context) = 0;

    virtual std::any visitCompoundStatement(TParser::CompoundStatementContext *context) = 0;

    virtual std::any visitBlockItemList(TParser::BlockItemListContext *context) = 0;

    virtual std::any visitBlockItem(TParser::BlockItemContext *context) = 0;

    virtual std::any visitExpressionStatement(TParser::ExpressionStatementContext *context) = 0;

    virtual std::any visitSelectionStatement(TParser::SelectionStatementContext *context) = 0;

    virtual std::any visitIterationStatement(TParser::IterationStatementContext *context) = 0;

    virtual std::any visitJumpStatement(TParser::JumpStatementContext *context) = 0;

    virtual std::any visitExpression(TParser::ExpressionContext *context) = 0;

    virtual std::any visitAssignmentExpression(TParser::AssignmentExpressionContext *context) = 0;

    virtual std::any visitConditionalExpression(TParser::ConditionalExpressionContext *context) = 0;

    virtual std::any visitConstantExpression(TParser::ConstantExpressionContext *context) = 0;

    virtual std::any visitLogicalOrExpression(TParser::LogicalOrExpressionContext *context) = 0;

    virtual std::any visitLogicalAndExpression(TParser::LogicalAndExpressionContext *context) = 0;

    virtual std::any visitInclusiveOrExpression(TParser::InclusiveOrExpressionContext *context) = 0;

    virtual std::any visitExclusiveOrExpression(TParser::ExclusiveOrExpressionContext *context) = 0;

    virtual std::any visitAndExpression(TParser::AndExpressionContext *context) = 0;

    virtual std::any visitEqualityExpression(TParser::EqualityExpressionContext *context) = 0;

    virtual std::any visitRelationalExpression(TParser::RelationalExpressionContext *context) = 0;

    virtual std::any visitShiftExpression(TParser::ShiftExpressionContext *context) = 0;

    virtual std::any visitAdditiveExpression(TParser::AdditiveExpressionContext *context) = 0;

    virtual std::any visitMultiplicativeExpression(TParser::MultiplicativeExpressionContext *context) = 0;

    virtual std::any visitCastExpression(TParser::CastExpressionContext *context) = 0;

    virtual std::any visitUnaryExpression(TParser::UnaryExpressionContext *context) = 0;

    virtual std::any visitUnaryOperator(TParser::UnaryOperatorContext *context) = 0;

    virtual std::any visitPostfixExpression(TParser::PostfixExpressionContext *context) = 0;

    virtual std::any visitPrimaryExpression(TParser::PrimaryExpressionContext *context) = 0;

    virtual std::any visitArgumentExpressionList(TParser::ArgumentExpressionListContext *context) = 0;

    virtual std::any visitConstant(TParser::ConstantContext *context) = 0;


};

}  // namespace antlrcpptest
