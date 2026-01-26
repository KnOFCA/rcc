
// Generated from ANTLR/TParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "TParser.h"


namespace antlrcpptest {

/**
 * This interface defines an abstract listener for a parse tree produced by TParser.
 */
class  TParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterTranslationUnit(TParser::TranslationUnitContext *ctx) = 0;
  virtual void exitTranslationUnit(TParser::TranslationUnitContext *ctx) = 0;

  virtual void enterExternalDeclaration(TParser::ExternalDeclarationContext *ctx) = 0;
  virtual void exitExternalDeclaration(TParser::ExternalDeclarationContext *ctx) = 0;

  virtual void enterFunctionDefinition(TParser::FunctionDefinitionContext *ctx) = 0;
  virtual void exitFunctionDefinition(TParser::FunctionDefinitionContext *ctx) = 0;

  virtual void enterDeclaration(TParser::DeclarationContext *ctx) = 0;
  virtual void exitDeclaration(TParser::DeclarationContext *ctx) = 0;

  virtual void enterDeclarationSpecifiers(TParser::DeclarationSpecifiersContext *ctx) = 0;
  virtual void exitDeclarationSpecifiers(TParser::DeclarationSpecifiersContext *ctx) = 0;

  virtual void enterDeclarationSpecifier(TParser::DeclarationSpecifierContext *ctx) = 0;
  virtual void exitDeclarationSpecifier(TParser::DeclarationSpecifierContext *ctx) = 0;

  virtual void enterStorageClassSpecifier(TParser::StorageClassSpecifierContext *ctx) = 0;
  virtual void exitStorageClassSpecifier(TParser::StorageClassSpecifierContext *ctx) = 0;

  virtual void enterTypeSpecifier(TParser::TypeSpecifierContext *ctx) = 0;
  virtual void exitTypeSpecifier(TParser::TypeSpecifierContext *ctx) = 0;

  virtual void enterTypeQualifier(TParser::TypeQualifierContext *ctx) = 0;
  virtual void exitTypeQualifier(TParser::TypeQualifierContext *ctx) = 0;

  virtual void enterStructSpecifier(TParser::StructSpecifierContext *ctx) = 0;
  virtual void exitStructSpecifier(TParser::StructSpecifierContext *ctx) = 0;

  virtual void enterUnionSpecifier(TParser::UnionSpecifierContext *ctx) = 0;
  virtual void exitUnionSpecifier(TParser::UnionSpecifierContext *ctx) = 0;

  virtual void enterStructDeclarationList(TParser::StructDeclarationListContext *ctx) = 0;
  virtual void exitStructDeclarationList(TParser::StructDeclarationListContext *ctx) = 0;

  virtual void enterStructDeclaration(TParser::StructDeclarationContext *ctx) = 0;
  virtual void exitStructDeclaration(TParser::StructDeclarationContext *ctx) = 0;

  virtual void enterStructDeclaratorList(TParser::StructDeclaratorListContext *ctx) = 0;
  virtual void exitStructDeclaratorList(TParser::StructDeclaratorListContext *ctx) = 0;

  virtual void enterStructDeclarator(TParser::StructDeclaratorContext *ctx) = 0;
  virtual void exitStructDeclarator(TParser::StructDeclaratorContext *ctx) = 0;

  virtual void enterEnumSpecifier(TParser::EnumSpecifierContext *ctx) = 0;
  virtual void exitEnumSpecifier(TParser::EnumSpecifierContext *ctx) = 0;

  virtual void enterEnumeratorList(TParser::EnumeratorListContext *ctx) = 0;
  virtual void exitEnumeratorList(TParser::EnumeratorListContext *ctx) = 0;

  virtual void enterEnumerator(TParser::EnumeratorContext *ctx) = 0;
  virtual void exitEnumerator(TParser::EnumeratorContext *ctx) = 0;

  virtual void enterInitDeclaratorList(TParser::InitDeclaratorListContext *ctx) = 0;
  virtual void exitInitDeclaratorList(TParser::InitDeclaratorListContext *ctx) = 0;

  virtual void enterInitDeclarator(TParser::InitDeclaratorContext *ctx) = 0;
  virtual void exitInitDeclarator(TParser::InitDeclaratorContext *ctx) = 0;

  virtual void enterDeclarator(TParser::DeclaratorContext *ctx) = 0;
  virtual void exitDeclarator(TParser::DeclaratorContext *ctx) = 0;

  virtual void enterDirectDeclarator(TParser::DirectDeclaratorContext *ctx) = 0;
  virtual void exitDirectDeclarator(TParser::DirectDeclaratorContext *ctx) = 0;

  virtual void enterPointer(TParser::PointerContext *ctx) = 0;
  virtual void exitPointer(TParser::PointerContext *ctx) = 0;

  virtual void enterParameterTypeList(TParser::ParameterTypeListContext *ctx) = 0;
  virtual void exitParameterTypeList(TParser::ParameterTypeListContext *ctx) = 0;

  virtual void enterParameterList(TParser::ParameterListContext *ctx) = 0;
  virtual void exitParameterList(TParser::ParameterListContext *ctx) = 0;

  virtual void enterParameterDeclaration(TParser::ParameterDeclarationContext *ctx) = 0;
  virtual void exitParameterDeclaration(TParser::ParameterDeclarationContext *ctx) = 0;

  virtual void enterInitializer(TParser::InitializerContext *ctx) = 0;
  virtual void exitInitializer(TParser::InitializerContext *ctx) = 0;

  virtual void enterInitializerList(TParser::InitializerListContext *ctx) = 0;
  virtual void exitInitializerList(TParser::InitializerListContext *ctx) = 0;

  virtual void enterStatement(TParser::StatementContext *ctx) = 0;
  virtual void exitStatement(TParser::StatementContext *ctx) = 0;

  virtual void enterLabeledStatement(TParser::LabeledStatementContext *ctx) = 0;
  virtual void exitLabeledStatement(TParser::LabeledStatementContext *ctx) = 0;

  virtual void enterCompoundStatement(TParser::CompoundStatementContext *ctx) = 0;
  virtual void exitCompoundStatement(TParser::CompoundStatementContext *ctx) = 0;

  virtual void enterBlockItemList(TParser::BlockItemListContext *ctx) = 0;
  virtual void exitBlockItemList(TParser::BlockItemListContext *ctx) = 0;

  virtual void enterBlockItem(TParser::BlockItemContext *ctx) = 0;
  virtual void exitBlockItem(TParser::BlockItemContext *ctx) = 0;

  virtual void enterExpressionStatement(TParser::ExpressionStatementContext *ctx) = 0;
  virtual void exitExpressionStatement(TParser::ExpressionStatementContext *ctx) = 0;

  virtual void enterSelectionStatement(TParser::SelectionStatementContext *ctx) = 0;
  virtual void exitSelectionStatement(TParser::SelectionStatementContext *ctx) = 0;

  virtual void enterIterationStatement(TParser::IterationStatementContext *ctx) = 0;
  virtual void exitIterationStatement(TParser::IterationStatementContext *ctx) = 0;

  virtual void enterJumpStatement(TParser::JumpStatementContext *ctx) = 0;
  virtual void exitJumpStatement(TParser::JumpStatementContext *ctx) = 0;

  virtual void enterExpression(TParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(TParser::ExpressionContext *ctx) = 0;

  virtual void enterAssignmentExpression(TParser::AssignmentExpressionContext *ctx) = 0;
  virtual void exitAssignmentExpression(TParser::AssignmentExpressionContext *ctx) = 0;

  virtual void enterConditionalExpression(TParser::ConditionalExpressionContext *ctx) = 0;
  virtual void exitConditionalExpression(TParser::ConditionalExpressionContext *ctx) = 0;

  virtual void enterConstantExpression(TParser::ConstantExpressionContext *ctx) = 0;
  virtual void exitConstantExpression(TParser::ConstantExpressionContext *ctx) = 0;

  virtual void enterLogicalOrExpression(TParser::LogicalOrExpressionContext *ctx) = 0;
  virtual void exitLogicalOrExpression(TParser::LogicalOrExpressionContext *ctx) = 0;

  virtual void enterLogicalAndExpression(TParser::LogicalAndExpressionContext *ctx) = 0;
  virtual void exitLogicalAndExpression(TParser::LogicalAndExpressionContext *ctx) = 0;

  virtual void enterInclusiveOrExpression(TParser::InclusiveOrExpressionContext *ctx) = 0;
  virtual void exitInclusiveOrExpression(TParser::InclusiveOrExpressionContext *ctx) = 0;

  virtual void enterExclusiveOrExpression(TParser::ExclusiveOrExpressionContext *ctx) = 0;
  virtual void exitExclusiveOrExpression(TParser::ExclusiveOrExpressionContext *ctx) = 0;

  virtual void enterAndExpression(TParser::AndExpressionContext *ctx) = 0;
  virtual void exitAndExpression(TParser::AndExpressionContext *ctx) = 0;

  virtual void enterEqualityExpression(TParser::EqualityExpressionContext *ctx) = 0;
  virtual void exitEqualityExpression(TParser::EqualityExpressionContext *ctx) = 0;

  virtual void enterRelationalExpression(TParser::RelationalExpressionContext *ctx) = 0;
  virtual void exitRelationalExpression(TParser::RelationalExpressionContext *ctx) = 0;

  virtual void enterShiftExpression(TParser::ShiftExpressionContext *ctx) = 0;
  virtual void exitShiftExpression(TParser::ShiftExpressionContext *ctx) = 0;

  virtual void enterAdditiveExpression(TParser::AdditiveExpressionContext *ctx) = 0;
  virtual void exitAdditiveExpression(TParser::AdditiveExpressionContext *ctx) = 0;

  virtual void enterMultiplicativeExpression(TParser::MultiplicativeExpressionContext *ctx) = 0;
  virtual void exitMultiplicativeExpression(TParser::MultiplicativeExpressionContext *ctx) = 0;

  virtual void enterCastExpression(TParser::CastExpressionContext *ctx) = 0;
  virtual void exitCastExpression(TParser::CastExpressionContext *ctx) = 0;

  virtual void enterUnaryExpression(TParser::UnaryExpressionContext *ctx) = 0;
  virtual void exitUnaryExpression(TParser::UnaryExpressionContext *ctx) = 0;

  virtual void enterUnaryOperator(TParser::UnaryOperatorContext *ctx) = 0;
  virtual void exitUnaryOperator(TParser::UnaryOperatorContext *ctx) = 0;

  virtual void enterPostfixExpression(TParser::PostfixExpressionContext *ctx) = 0;
  virtual void exitPostfixExpression(TParser::PostfixExpressionContext *ctx) = 0;

  virtual void enterPrimaryExpression(TParser::PrimaryExpressionContext *ctx) = 0;
  virtual void exitPrimaryExpression(TParser::PrimaryExpressionContext *ctx) = 0;

  virtual void enterArgumentExpressionList(TParser::ArgumentExpressionListContext *ctx) = 0;
  virtual void exitArgumentExpressionList(TParser::ArgumentExpressionListContext *ctx) = 0;

  virtual void enterConstant(TParser::ConstantContext *ctx) = 0;
  virtual void exitConstant(TParser::ConstantContext *ctx) = 0;


};

}  // namespace antlrcpptest
