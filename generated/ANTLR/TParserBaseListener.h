
// Generated from ANTLR/TParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "TParserListener.h"


namespace antlrcpptest {

/**
 * This class provides an empty implementation of TParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  TParserBaseListener : public TParserListener {
public:

  virtual void enterTranslationUnit(TParser::TranslationUnitContext * /*ctx*/) override { }
  virtual void exitTranslationUnit(TParser::TranslationUnitContext * /*ctx*/) override { }

  virtual void enterExternalDeclaration(TParser::ExternalDeclarationContext * /*ctx*/) override { }
  virtual void exitExternalDeclaration(TParser::ExternalDeclarationContext * /*ctx*/) override { }

  virtual void enterFunctionDefinition(TParser::FunctionDefinitionContext * /*ctx*/) override { }
  virtual void exitFunctionDefinition(TParser::FunctionDefinitionContext * /*ctx*/) override { }

  virtual void enterDeclaration(TParser::DeclarationContext * /*ctx*/) override { }
  virtual void exitDeclaration(TParser::DeclarationContext * /*ctx*/) override { }

  virtual void enterDeclarationSpecifiers(TParser::DeclarationSpecifiersContext * /*ctx*/) override { }
  virtual void exitDeclarationSpecifiers(TParser::DeclarationSpecifiersContext * /*ctx*/) override { }

  virtual void enterDeclarationSpecifier(TParser::DeclarationSpecifierContext * /*ctx*/) override { }
  virtual void exitDeclarationSpecifier(TParser::DeclarationSpecifierContext * /*ctx*/) override { }

  virtual void enterStorageClassSpecifier(TParser::StorageClassSpecifierContext * /*ctx*/) override { }
  virtual void exitStorageClassSpecifier(TParser::StorageClassSpecifierContext * /*ctx*/) override { }

  virtual void enterTypeSpecifier(TParser::TypeSpecifierContext * /*ctx*/) override { }
  virtual void exitTypeSpecifier(TParser::TypeSpecifierContext * /*ctx*/) override { }

  virtual void enterTypeQualifier(TParser::TypeQualifierContext * /*ctx*/) override { }
  virtual void exitTypeQualifier(TParser::TypeQualifierContext * /*ctx*/) override { }

  virtual void enterStructSpecifier(TParser::StructSpecifierContext * /*ctx*/) override { }
  virtual void exitStructSpecifier(TParser::StructSpecifierContext * /*ctx*/) override { }

  virtual void enterUnionSpecifier(TParser::UnionSpecifierContext * /*ctx*/) override { }
  virtual void exitUnionSpecifier(TParser::UnionSpecifierContext * /*ctx*/) override { }

  virtual void enterStructDeclarationList(TParser::StructDeclarationListContext * /*ctx*/) override { }
  virtual void exitStructDeclarationList(TParser::StructDeclarationListContext * /*ctx*/) override { }

  virtual void enterStructDeclaration(TParser::StructDeclarationContext * /*ctx*/) override { }
  virtual void exitStructDeclaration(TParser::StructDeclarationContext * /*ctx*/) override { }

  virtual void enterStructDeclaratorList(TParser::StructDeclaratorListContext * /*ctx*/) override { }
  virtual void exitStructDeclaratorList(TParser::StructDeclaratorListContext * /*ctx*/) override { }

  virtual void enterStructDeclarator(TParser::StructDeclaratorContext * /*ctx*/) override { }
  virtual void exitStructDeclarator(TParser::StructDeclaratorContext * /*ctx*/) override { }

  virtual void enterEnumSpecifier(TParser::EnumSpecifierContext * /*ctx*/) override { }
  virtual void exitEnumSpecifier(TParser::EnumSpecifierContext * /*ctx*/) override { }

  virtual void enterEnumeratorList(TParser::EnumeratorListContext * /*ctx*/) override { }
  virtual void exitEnumeratorList(TParser::EnumeratorListContext * /*ctx*/) override { }

  virtual void enterEnumerator(TParser::EnumeratorContext * /*ctx*/) override { }
  virtual void exitEnumerator(TParser::EnumeratorContext * /*ctx*/) override { }

  virtual void enterInitDeclaratorList(TParser::InitDeclaratorListContext * /*ctx*/) override { }
  virtual void exitInitDeclaratorList(TParser::InitDeclaratorListContext * /*ctx*/) override { }

  virtual void enterInitDeclarator(TParser::InitDeclaratorContext * /*ctx*/) override { }
  virtual void exitInitDeclarator(TParser::InitDeclaratorContext * /*ctx*/) override { }

  virtual void enterDeclarator(TParser::DeclaratorContext * /*ctx*/) override { }
  virtual void exitDeclarator(TParser::DeclaratorContext * /*ctx*/) override { }

  virtual void enterDirectDeclarator(TParser::DirectDeclaratorContext * /*ctx*/) override { }
  virtual void exitDirectDeclarator(TParser::DirectDeclaratorContext * /*ctx*/) override { }

  virtual void enterPointer(TParser::PointerContext * /*ctx*/) override { }
  virtual void exitPointer(TParser::PointerContext * /*ctx*/) override { }

  virtual void enterParameterTypeList(TParser::ParameterTypeListContext * /*ctx*/) override { }
  virtual void exitParameterTypeList(TParser::ParameterTypeListContext * /*ctx*/) override { }

  virtual void enterParameterList(TParser::ParameterListContext * /*ctx*/) override { }
  virtual void exitParameterList(TParser::ParameterListContext * /*ctx*/) override { }

  virtual void enterParameterDeclaration(TParser::ParameterDeclarationContext * /*ctx*/) override { }
  virtual void exitParameterDeclaration(TParser::ParameterDeclarationContext * /*ctx*/) override { }

  virtual void enterInitializer(TParser::InitializerContext * /*ctx*/) override { }
  virtual void exitInitializer(TParser::InitializerContext * /*ctx*/) override { }

  virtual void enterInitializerList(TParser::InitializerListContext * /*ctx*/) override { }
  virtual void exitInitializerList(TParser::InitializerListContext * /*ctx*/) override { }

  virtual void enterStatement(TParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(TParser::StatementContext * /*ctx*/) override { }

  virtual void enterLabeledStatement(TParser::LabeledStatementContext * /*ctx*/) override { }
  virtual void exitLabeledStatement(TParser::LabeledStatementContext * /*ctx*/) override { }

  virtual void enterCompoundStatement(TParser::CompoundStatementContext * /*ctx*/) override { }
  virtual void exitCompoundStatement(TParser::CompoundStatementContext * /*ctx*/) override { }

  virtual void enterBlockItemList(TParser::BlockItemListContext * /*ctx*/) override { }
  virtual void exitBlockItemList(TParser::BlockItemListContext * /*ctx*/) override { }

  virtual void enterBlockItem(TParser::BlockItemContext * /*ctx*/) override { }
  virtual void exitBlockItem(TParser::BlockItemContext * /*ctx*/) override { }

  virtual void enterExpressionStatement(TParser::ExpressionStatementContext * /*ctx*/) override { }
  virtual void exitExpressionStatement(TParser::ExpressionStatementContext * /*ctx*/) override { }

  virtual void enterSelectionStatement(TParser::SelectionStatementContext * /*ctx*/) override { }
  virtual void exitSelectionStatement(TParser::SelectionStatementContext * /*ctx*/) override { }

  virtual void enterIterationStatement(TParser::IterationStatementContext * /*ctx*/) override { }
  virtual void exitIterationStatement(TParser::IterationStatementContext * /*ctx*/) override { }

  virtual void enterJumpStatement(TParser::JumpStatementContext * /*ctx*/) override { }
  virtual void exitJumpStatement(TParser::JumpStatementContext * /*ctx*/) override { }

  virtual void enterExpression(TParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(TParser::ExpressionContext * /*ctx*/) override { }

  virtual void enterAssignmentExpression(TParser::AssignmentExpressionContext * /*ctx*/) override { }
  virtual void exitAssignmentExpression(TParser::AssignmentExpressionContext * /*ctx*/) override { }

  virtual void enterConditionalExpression(TParser::ConditionalExpressionContext * /*ctx*/) override { }
  virtual void exitConditionalExpression(TParser::ConditionalExpressionContext * /*ctx*/) override { }

  virtual void enterConstantExpression(TParser::ConstantExpressionContext * /*ctx*/) override { }
  virtual void exitConstantExpression(TParser::ConstantExpressionContext * /*ctx*/) override { }

  virtual void enterLogicalOrExpression(TParser::LogicalOrExpressionContext * /*ctx*/) override { }
  virtual void exitLogicalOrExpression(TParser::LogicalOrExpressionContext * /*ctx*/) override { }

  virtual void enterLogicalAndExpression(TParser::LogicalAndExpressionContext * /*ctx*/) override { }
  virtual void exitLogicalAndExpression(TParser::LogicalAndExpressionContext * /*ctx*/) override { }

  virtual void enterInclusiveOrExpression(TParser::InclusiveOrExpressionContext * /*ctx*/) override { }
  virtual void exitInclusiveOrExpression(TParser::InclusiveOrExpressionContext * /*ctx*/) override { }

  virtual void enterExclusiveOrExpression(TParser::ExclusiveOrExpressionContext * /*ctx*/) override { }
  virtual void exitExclusiveOrExpression(TParser::ExclusiveOrExpressionContext * /*ctx*/) override { }

  virtual void enterAndExpression(TParser::AndExpressionContext * /*ctx*/) override { }
  virtual void exitAndExpression(TParser::AndExpressionContext * /*ctx*/) override { }

  virtual void enterEqualityExpression(TParser::EqualityExpressionContext * /*ctx*/) override { }
  virtual void exitEqualityExpression(TParser::EqualityExpressionContext * /*ctx*/) override { }

  virtual void enterRelationalExpression(TParser::RelationalExpressionContext * /*ctx*/) override { }
  virtual void exitRelationalExpression(TParser::RelationalExpressionContext * /*ctx*/) override { }

  virtual void enterShiftExpression(TParser::ShiftExpressionContext * /*ctx*/) override { }
  virtual void exitShiftExpression(TParser::ShiftExpressionContext * /*ctx*/) override { }

  virtual void enterAdditiveExpression(TParser::AdditiveExpressionContext * /*ctx*/) override { }
  virtual void exitAdditiveExpression(TParser::AdditiveExpressionContext * /*ctx*/) override { }

  virtual void enterMultiplicativeExpression(TParser::MultiplicativeExpressionContext * /*ctx*/) override { }
  virtual void exitMultiplicativeExpression(TParser::MultiplicativeExpressionContext * /*ctx*/) override { }

  virtual void enterCastExpression(TParser::CastExpressionContext * /*ctx*/) override { }
  virtual void exitCastExpression(TParser::CastExpressionContext * /*ctx*/) override { }

  virtual void enterUnaryExpression(TParser::UnaryExpressionContext * /*ctx*/) override { }
  virtual void exitUnaryExpression(TParser::UnaryExpressionContext * /*ctx*/) override { }

  virtual void enterUnaryOperator(TParser::UnaryOperatorContext * /*ctx*/) override { }
  virtual void exitUnaryOperator(TParser::UnaryOperatorContext * /*ctx*/) override { }

  virtual void enterPostfixExpression(TParser::PostfixExpressionContext * /*ctx*/) override { }
  virtual void exitPostfixExpression(TParser::PostfixExpressionContext * /*ctx*/) override { }

  virtual void enterPrimaryExpression(TParser::PrimaryExpressionContext * /*ctx*/) override { }
  virtual void exitPrimaryExpression(TParser::PrimaryExpressionContext * /*ctx*/) override { }

  virtual void enterArgumentExpressionList(TParser::ArgumentExpressionListContext * /*ctx*/) override { }
  virtual void exitArgumentExpressionList(TParser::ArgumentExpressionListContext * /*ctx*/) override { }

  virtual void enterConstant(TParser::ConstantContext * /*ctx*/) override { }
  virtual void exitConstant(TParser::ConstantContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

}  // namespace antlrcpptest
