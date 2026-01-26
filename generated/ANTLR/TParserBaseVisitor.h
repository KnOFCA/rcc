
// Generated from ANTLR/TParser.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "TParserVisitor.h"


namespace antlrcpptest {

/**
 * This class provides an empty implementation of TParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  TParserBaseVisitor : public TParserVisitor {
public:

  virtual std::any visitTranslationUnit(TParser::TranslationUnitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExternalDeclaration(TParser::ExternalDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunctionDefinition(TParser::FunctionDefinitionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclaration(TParser::DeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclarationSpecifiers(TParser::DeclarationSpecifiersContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclarationSpecifier(TParser::DeclarationSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStorageClassSpecifier(TParser::StorageClassSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeSpecifier(TParser::TypeSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitTypeQualifier(TParser::TypeQualifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructSpecifier(TParser::StructSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnionSpecifier(TParser::UnionSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructDeclarationList(TParser::StructDeclarationListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructDeclaration(TParser::StructDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructDeclaratorList(TParser::StructDeclaratorListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStructDeclarator(TParser::StructDeclaratorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEnumSpecifier(TParser::EnumSpecifierContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEnumeratorList(TParser::EnumeratorListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEnumerator(TParser::EnumeratorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInitDeclaratorList(TParser::InitDeclaratorListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInitDeclarator(TParser::InitDeclaratorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclarator(TParser::DeclaratorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDirectDeclarator(TParser::DirectDeclaratorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPointer(TParser::PointerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParameterTypeList(TParser::ParameterTypeListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParameterList(TParser::ParameterListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParameterDeclaration(TParser::ParameterDeclarationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInitializer(TParser::InitializerContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInitializerList(TParser::InitializerListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStatement(TParser::StatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLabeledStatement(TParser::LabeledStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCompoundStatement(TParser::CompoundStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockItemList(TParser::BlockItemListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlockItem(TParser::BlockItemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpressionStatement(TParser::ExpressionStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSelectionStatement(TParser::SelectionStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitIterationStatement(TParser::IterationStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitJumpStatement(TParser::JumpStatementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExpression(TParser::ExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssignmentExpression(TParser::AssignmentExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConditionalExpression(TParser::ConditionalExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstantExpression(TParser::ConstantExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicalOrExpression(TParser::LogicalOrExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLogicalAndExpression(TParser::LogicalAndExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitInclusiveOrExpression(TParser::InclusiveOrExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitExclusiveOrExpression(TParser::ExclusiveOrExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAndExpression(TParser::AndExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitEqualityExpression(TParser::EqualityExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitRelationalExpression(TParser::RelationalExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitShiftExpression(TParser::ShiftExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAdditiveExpression(TParser::AdditiveExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMultiplicativeExpression(TParser::MultiplicativeExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitCastExpression(TParser::CastExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryExpression(TParser::UnaryExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitUnaryOperator(TParser::UnaryOperatorContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPostfixExpression(TParser::PostfixExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitPrimaryExpression(TParser::PrimaryExpressionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitArgumentExpressionList(TParser::ArgumentExpressionListContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitConstant(TParser::ConstantContext *ctx) override {
    return visitChildren(ctx);
  }


};

}  // namespace antlrcpptest
