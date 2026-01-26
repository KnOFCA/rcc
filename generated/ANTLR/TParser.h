/* parser/listener/visitor header section */

// Generated from ANTLR/TParser.g4 by ANTLR 4.13.2

#pragma once

/* parser precinclude section */

#include "antlr4-runtime.h"


/* parser postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


namespace antlrcpptest {

/* parser context section */

class  TParser : public antlr4::Parser {
public:
  enum {
    DUMMY = 1, Auto = 2, Break = 3, Case = 4, Char = 5, Const = 6, Continue = 7, 
    Default = 8, Do = 9, Double = 10, Else = 11, Enum = 12, Extern = 13, 
    Float = 14, For = 15, Goto = 16, If = 17, Inline = 18, Int = 19, Long = 20, 
    Register = 21, Restrict = 22, Return = 23, Short = 24, Signed = 25, 
    Sizeof = 26, Static = 27, Struct = 28, Switch = 29, Typedef = 30, Union = 31, 
    Unsigned = 32, Void = 33, Volatile = 34, While = 35, Alignas = 36, Alignof = 37, 
    Atomic = 38, Bool = 39, Complex = 40, Generic = 41, Imaginary = 42, 
    Noreturn = 43, StaticAssert = 44, ThreadLocal = 45, CxxClass = 46, CxxNamespace = 47, 
    Identifier = 48, IntegerConstant = 49, FloatingConstant = 50, CharacterConstant = 51, 
    StringLiteral = 52, LeftParen = 53, RightParen = 54, LeftBracket = 55, 
    RightBracket = 56, LeftBrace = 57, RightBrace = 58, Dot = 59, Arrow = 60, 
    PlusPlus = 61, MinusMinus = 62, Plus = 63, Minus = 64, Star = 65, Slash = 66, 
    Percent = 67, And = 68, Or = 69, Caret = 70, Tilde = 71, Not = 72, Assign = 73, 
    Less = 74, Greater = 75, LessEqual = 76, GreaterEqual = 77, Equal = 78, 
    NotEqual = 79, AndAnd = 80, OrOr = 81, Question = 82, Colon = 83, Semi = 84, 
    Comma = 85, Ellipsis = 86, Preprocessor = 87, LineComment = 88, BlockComment = 89, 
    Whitespace = 90
  };

  enum {
    RuleTranslationUnit = 0, RuleExternalDeclaration = 1, RuleFunctionDefinition = 2, 
    RuleDeclaration = 3, RuleDeclarationSpecifiers = 4, RuleDeclarationSpecifier = 5, 
    RuleStorageClassSpecifier = 6, RuleTypeSpecifier = 7, RuleTypeQualifier = 8, 
    RuleStructSpecifier = 9, RuleUnionSpecifier = 10, RuleStructDeclarationList = 11, 
    RuleStructDeclaration = 12, RuleStructDeclaratorList = 13, RuleStructDeclarator = 14, 
    RuleEnumSpecifier = 15, RuleEnumeratorList = 16, RuleEnumerator = 17, 
    RuleInitDeclaratorList = 18, RuleInitDeclarator = 19, RuleDeclarator = 20, 
    RuleDirectDeclarator = 21, RulePointer = 22, RuleParameterTypeList = 23, 
    RuleParameterList = 24, RuleParameterDeclaration = 25, RuleInitializer = 26, 
    RuleInitializerList = 27, RuleStatement = 28, RuleLabeledStatement = 29, 
    RuleCompoundStatement = 30, RuleBlockItemList = 31, RuleBlockItem = 32, 
    RuleExpressionStatement = 33, RuleSelectionStatement = 34, RuleIterationStatement = 35, 
    RuleJumpStatement = 36, RuleExpression = 37, RuleAssignmentExpression = 38, 
    RuleConditionalExpression = 39, RuleConstantExpression = 40, RuleLogicalOrExpression = 41, 
    RuleLogicalAndExpression = 42, RuleInclusiveOrExpression = 43, RuleExclusiveOrExpression = 44, 
    RuleAndExpression = 45, RuleEqualityExpression = 46, RuleRelationalExpression = 47, 
    RuleShiftExpression = 48, RuleAdditiveExpression = 49, RuleMultiplicativeExpression = 50, 
    RuleCastExpression = 51, RuleUnaryExpression = 52, RuleUnaryOperator = 53, 
    RulePostfixExpression = 54, RulePrimaryExpression = 55, RuleArgumentExpressionList = 56, 
    RuleConstant = 57
  };

  explicit TParser(antlr4::TokenStream *input);

  TParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~TParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  /* public parser declarations/members section */
  bool myAction() { return true; }
  bool doesItBlend() { return true; }
  void cleanUp() {}
  void doInit() {}
  void doAfter() {}


  class TranslationUnitContext;
  class ExternalDeclarationContext;
  class FunctionDefinitionContext;
  class DeclarationContext;
  class DeclarationSpecifiersContext;
  class DeclarationSpecifierContext;
  class StorageClassSpecifierContext;
  class TypeSpecifierContext;
  class TypeQualifierContext;
  class StructSpecifierContext;
  class UnionSpecifierContext;
  class StructDeclarationListContext;
  class StructDeclarationContext;
  class StructDeclaratorListContext;
  class StructDeclaratorContext;
  class EnumSpecifierContext;
  class EnumeratorListContext;
  class EnumeratorContext;
  class InitDeclaratorListContext;
  class InitDeclaratorContext;
  class DeclaratorContext;
  class DirectDeclaratorContext;
  class PointerContext;
  class ParameterTypeListContext;
  class ParameterListContext;
  class ParameterDeclarationContext;
  class InitializerContext;
  class InitializerListContext;
  class StatementContext;
  class LabeledStatementContext;
  class CompoundStatementContext;
  class BlockItemListContext;
  class BlockItemContext;
  class ExpressionStatementContext;
  class SelectionStatementContext;
  class IterationStatementContext;
  class JumpStatementContext;
  class ExpressionContext;
  class AssignmentExpressionContext;
  class ConditionalExpressionContext;
  class ConstantExpressionContext;
  class LogicalOrExpressionContext;
  class LogicalAndExpressionContext;
  class InclusiveOrExpressionContext;
  class ExclusiveOrExpressionContext;
  class AndExpressionContext;
  class EqualityExpressionContext;
  class RelationalExpressionContext;
  class ShiftExpressionContext;
  class AdditiveExpressionContext;
  class MultiplicativeExpressionContext;
  class CastExpressionContext;
  class UnaryExpressionContext;
  class UnaryOperatorContext;
  class PostfixExpressionContext;
  class PrimaryExpressionContext;
  class ArgumentExpressionListContext;
  class ConstantContext; 

  class  TranslationUnitContext : public antlr4::ParserRuleContext {
  public:
    TranslationUnitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExternalDeclarationContext *> externalDeclaration();
    ExternalDeclarationContext* externalDeclaration(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TranslationUnitContext* translationUnit();

  class  ExternalDeclarationContext : public antlr4::ParserRuleContext {
  public:
    ExternalDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FunctionDefinitionContext *functionDefinition();
    DeclarationContext *declaration();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExternalDeclarationContext* externalDeclaration();

  class  FunctionDefinitionContext : public antlr4::ParserRuleContext {
  public:
    FunctionDefinitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclarationSpecifiersContext *declarationSpecifiers();
    DeclaratorContext *declarator();
    CompoundStatementContext *compoundStatement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionDefinitionContext* functionDefinition();

  class  DeclarationContext : public antlr4::ParserRuleContext {
  public:
    DeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclarationSpecifiersContext *declarationSpecifiers();
    antlr4::tree::TerminalNode *Semi();
    InitDeclaratorListContext *initDeclaratorList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationContext* declaration();

  class  DeclarationSpecifiersContext : public antlr4::ParserRuleContext {
  public:
    DeclarationSpecifiersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclarationSpecifierContext *> declarationSpecifier();
    DeclarationSpecifierContext* declarationSpecifier(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationSpecifiersContext* declarationSpecifiers();

  class  DeclarationSpecifierContext : public antlr4::ParserRuleContext {
  public:
    DeclarationSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    StorageClassSpecifierContext *storageClassSpecifier();
    TypeSpecifierContext *typeSpecifier();
    TypeQualifierContext *typeQualifier();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclarationSpecifierContext* declarationSpecifier();

  class  StorageClassSpecifierContext : public antlr4::ParserRuleContext {
  public:
    StorageClassSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Auto();
    antlr4::tree::TerminalNode *Register();
    antlr4::tree::TerminalNode *Static();
    antlr4::tree::TerminalNode *Extern();
    antlr4::tree::TerminalNode *Typedef();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StorageClassSpecifierContext* storageClassSpecifier();

  class  TypeSpecifierContext : public antlr4::ParserRuleContext {
  public:
    TypeSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Void();
    antlr4::tree::TerminalNode *Char();
    antlr4::tree::TerminalNode *Short();
    antlr4::tree::TerminalNode *Int();
    antlr4::tree::TerminalNode *Long();
    antlr4::tree::TerminalNode *Float();
    antlr4::tree::TerminalNode *Double();
    antlr4::tree::TerminalNode *Signed();
    antlr4::tree::TerminalNode *Unsigned();
    antlr4::tree::TerminalNode *Bool();
    StructSpecifierContext *structSpecifier();
    UnionSpecifierContext *unionSpecifier();
    EnumSpecifierContext *enumSpecifier();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeSpecifierContext* typeSpecifier();

  class  TypeQualifierContext : public antlr4::ParserRuleContext {
  public:
    TypeQualifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Const();
    antlr4::tree::TerminalNode *Volatile();
    antlr4::tree::TerminalNode *Restrict();
    antlr4::tree::TerminalNode *Atomic();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TypeQualifierContext* typeQualifier();

  class  StructSpecifierContext : public antlr4::ParserRuleContext {
  public:
    StructSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Struct();
    antlr4::tree::TerminalNode *LeftBrace();
    StructDeclarationListContext *structDeclarationList();
    antlr4::tree::TerminalNode *RightBrace();
    antlr4::tree::TerminalNode *Identifier();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StructSpecifierContext* structSpecifier();

  class  UnionSpecifierContext : public antlr4::ParserRuleContext {
  public:
    UnionSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Union();
    antlr4::tree::TerminalNode *LeftBrace();
    StructDeclarationListContext *structDeclarationList();
    antlr4::tree::TerminalNode *RightBrace();
    antlr4::tree::TerminalNode *Identifier();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnionSpecifierContext* unionSpecifier();

  class  StructDeclarationListContext : public antlr4::ParserRuleContext {
  public:
    StructDeclarationListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StructDeclarationContext *> structDeclaration();
    StructDeclarationContext* structDeclaration(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StructDeclarationListContext* structDeclarationList();

  class  StructDeclarationContext : public antlr4::ParserRuleContext {
  public:
    StructDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclarationSpecifiersContext *declarationSpecifiers();
    StructDeclaratorListContext *structDeclaratorList();
    antlr4::tree::TerminalNode *Semi();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StructDeclarationContext* structDeclaration();

  class  StructDeclaratorListContext : public antlr4::ParserRuleContext {
  public:
    StructDeclaratorListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<StructDeclaratorContext *> structDeclarator();
    StructDeclaratorContext* structDeclarator(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StructDeclaratorListContext* structDeclaratorList();

  class  StructDeclaratorContext : public antlr4::ParserRuleContext {
  public:
    StructDeclaratorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclaratorContext *declarator();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StructDeclaratorContext* structDeclarator();

  class  EnumSpecifierContext : public antlr4::ParserRuleContext {
  public:
    EnumSpecifierContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Enum();
    antlr4::tree::TerminalNode *LeftBrace();
    EnumeratorListContext *enumeratorList();
    antlr4::tree::TerminalNode *RightBrace();
    antlr4::tree::TerminalNode *Identifier();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumSpecifierContext* enumSpecifier();

  class  EnumeratorListContext : public antlr4::ParserRuleContext {
  public:
    EnumeratorListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EnumeratorContext *> enumerator();
    EnumeratorContext* enumerator(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumeratorListContext* enumeratorList();

  class  EnumeratorContext : public antlr4::ParserRuleContext {
  public:
    EnumeratorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *Assign();
    ConstantExpressionContext *constantExpression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EnumeratorContext* enumerator();

  class  InitDeclaratorListContext : public antlr4::ParserRuleContext {
  public:
    InitDeclaratorListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<InitDeclaratorContext *> initDeclarator();
    InitDeclaratorContext* initDeclarator(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InitDeclaratorListContext* initDeclaratorList();

  class  InitDeclaratorContext : public antlr4::ParserRuleContext {
  public:
    InitDeclaratorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclaratorContext *declarator();
    antlr4::tree::TerminalNode *Assign();
    InitializerContext *initializer();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InitDeclaratorContext* initDeclarator();

  class  DeclaratorContext : public antlr4::ParserRuleContext {
  public:
    DeclaratorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DirectDeclaratorContext *directDeclarator();
    PointerContext *pointer();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclaratorContext* declarator();

  class  DirectDeclaratorContext : public antlr4::ParserRuleContext {
  public:
    DirectDeclaratorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *LeftParen();
    DeclaratorContext *declarator();
    antlr4::tree::TerminalNode *RightParen();
    DirectDeclaratorContext *directDeclarator();
    ParameterTypeListContext *parameterTypeList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DirectDeclaratorContext* directDeclarator();
  DirectDeclaratorContext* directDeclarator(int precedence);
  class  PointerContext : public antlr4::ParserRuleContext {
  public:
    PointerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Star();
    std::vector<TypeQualifierContext *> typeQualifier();
    TypeQualifierContext* typeQualifier(size_t i);
    PointerContext *pointer();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PointerContext* pointer();

  class  ParameterTypeListContext : public antlr4::ParserRuleContext {
  public:
    ParameterTypeListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ParameterListContext *parameterList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterTypeListContext* parameterTypeList();

  class  ParameterListContext : public antlr4::ParserRuleContext {
  public:
    ParameterListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ParameterDeclarationContext *> parameterDeclaration();
    ParameterDeclarationContext* parameterDeclaration(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterListContext* parameterList();

  class  ParameterDeclarationContext : public antlr4::ParserRuleContext {
  public:
    ParameterDeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclarationSpecifiersContext *declarationSpecifiers();
    DeclaratorContext *declarator();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ParameterDeclarationContext* parameterDeclaration();

  class  InitializerContext : public antlr4::ParserRuleContext {
  public:
    InitializerContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AssignmentExpressionContext *assignmentExpression();
    antlr4::tree::TerminalNode *LeftBrace();
    InitializerListContext *initializerList();
    antlr4::tree::TerminalNode *RightBrace();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InitializerContext* initializer();

  class  InitializerListContext : public antlr4::ParserRuleContext {
  public:
    InitializerListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<InitializerContext *> initializer();
    InitializerContext* initializer(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InitializerListContext* initializerList();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LabeledStatementContext *labeledStatement();
    CompoundStatementContext *compoundStatement();
    ExpressionStatementContext *expressionStatement();
    SelectionStatementContext *selectionStatement();
    IterationStatementContext *iterationStatement();
    JumpStatementContext *jumpStatement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StatementContext* statement();

  class  LabeledStatementContext : public antlr4::ParserRuleContext {
  public:
    LabeledStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *Colon();
    StatementContext *statement();
    antlr4::tree::TerminalNode *Case();
    ConstantExpressionContext *constantExpression();
    antlr4::tree::TerminalNode *Default();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabeledStatementContext* labeledStatement();

  class  CompoundStatementContext : public antlr4::ParserRuleContext {
  public:
    CompoundStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LeftBrace();
    antlr4::tree::TerminalNode *RightBrace();
    BlockItemListContext *blockItemList();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CompoundStatementContext* compoundStatement();

  class  BlockItemListContext : public antlr4::ParserRuleContext {
  public:
    BlockItemListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<BlockItemContext *> blockItem();
    BlockItemContext* blockItem(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlockItemListContext* blockItemList();

  class  BlockItemContext : public antlr4::ParserRuleContext {
  public:
    BlockItemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DeclarationContext *declaration();
    StatementContext *statement();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlockItemContext* blockItem();

  class  ExpressionStatementContext : public antlr4::ParserRuleContext {
  public:
    ExpressionStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Semi();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionStatementContext* expressionStatement();

  class  SelectionStatementContext : public antlr4::ParserRuleContext {
  public:
    SelectionStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *If();
    antlr4::tree::TerminalNode *LeftParen();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RightParen();
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);
    antlr4::tree::TerminalNode *Else();
    antlr4::tree::TerminalNode *Switch();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SelectionStatementContext* selectionStatement();

  class  IterationStatementContext : public antlr4::ParserRuleContext {
  public:
    IterationStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *While();
    antlr4::tree::TerminalNode *LeftParen();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RightParen();
    StatementContext *statement();
    antlr4::tree::TerminalNode *Do();
    antlr4::tree::TerminalNode *Semi();
    antlr4::tree::TerminalNode *For();
    std::vector<ExpressionStatementContext *> expressionStatement();
    ExpressionStatementContext* expressionStatement(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IterationStatementContext* iterationStatement();

  class  JumpStatementContext : public antlr4::ParserRuleContext {
  public:
    JumpStatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Goto();
    antlr4::tree::TerminalNode *Identifier();
    antlr4::tree::TerminalNode *Semi();
    antlr4::tree::TerminalNode *Continue();
    antlr4::tree::TerminalNode *Break();
    antlr4::tree::TerminalNode *Return();
    ExpressionContext *expression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  JumpStatementContext* jumpStatement();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AssignmentExpressionContext *> assignmentExpression();
    AssignmentExpressionContext* assignmentExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpressionContext* expression();

  class  AssignmentExpressionContext : public antlr4::ParserRuleContext {
  public:
    AssignmentExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConditionalExpressionContext *conditionalExpression();
    UnaryExpressionContext *unaryExpression();
    antlr4::tree::TerminalNode *Assign();
    AssignmentExpressionContext *assignmentExpression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AssignmentExpressionContext* assignmentExpression();

  class  ConditionalExpressionContext : public antlr4::ParserRuleContext {
  public:
    ConditionalExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    LogicalOrExpressionContext *logicalOrExpression();
    antlr4::tree::TerminalNode *Question();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *Colon();
    ConditionalExpressionContext *conditionalExpression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConditionalExpressionContext* conditionalExpression();

  class  ConstantExpressionContext : public antlr4::ParserRuleContext {
  public:
    ConstantExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConditionalExpressionContext *conditionalExpression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstantExpressionContext* constantExpression();

  class  LogicalOrExpressionContext : public antlr4::ParserRuleContext {
  public:
    LogicalOrExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LogicalAndExpressionContext *> logicalAndExpression();
    LogicalAndExpressionContext* logicalAndExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> OrOr();
    antlr4::tree::TerminalNode* OrOr(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LogicalOrExpressionContext* logicalOrExpression();

  class  LogicalAndExpressionContext : public antlr4::ParserRuleContext {
  public:
    LogicalAndExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<InclusiveOrExpressionContext *> inclusiveOrExpression();
    InclusiveOrExpressionContext* inclusiveOrExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> AndAnd();
    antlr4::tree::TerminalNode* AndAnd(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LogicalAndExpressionContext* logicalAndExpression();

  class  InclusiveOrExpressionContext : public antlr4::ParserRuleContext {
  public:
    InclusiveOrExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExclusiveOrExpressionContext *> exclusiveOrExpression();
    ExclusiveOrExpressionContext* exclusiveOrExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Or();
    antlr4::tree::TerminalNode* Or(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InclusiveOrExpressionContext* inclusiveOrExpression();

  class  ExclusiveOrExpressionContext : public antlr4::ParserRuleContext {
  public:
    ExclusiveOrExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AndExpressionContext *> andExpression();
    AndExpressionContext* andExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Caret();
    antlr4::tree::TerminalNode* Caret(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExclusiveOrExpressionContext* exclusiveOrExpression();

  class  AndExpressionContext : public antlr4::ParserRuleContext {
  public:
    AndExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EqualityExpressionContext *> equalityExpression();
    EqualityExpressionContext* equalityExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> And();
    antlr4::tree::TerminalNode* And(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AndExpressionContext* andExpression();

  class  EqualityExpressionContext : public antlr4::ParserRuleContext {
  public:
    EqualityExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<RelationalExpressionContext *> relationalExpression();
    RelationalExpressionContext* relationalExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Equal();
    antlr4::tree::TerminalNode* Equal(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NotEqual();
    antlr4::tree::TerminalNode* NotEqual(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EqualityExpressionContext* equalityExpression();

  class  RelationalExpressionContext : public antlr4::ParserRuleContext {
  public:
    RelationalExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ShiftExpressionContext *> shiftExpression();
    ShiftExpressionContext* shiftExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Less();
    antlr4::tree::TerminalNode* Less(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Greater();
    antlr4::tree::TerminalNode* Greater(size_t i);
    std::vector<antlr4::tree::TerminalNode *> LessEqual();
    antlr4::tree::TerminalNode* LessEqual(size_t i);
    std::vector<antlr4::tree::TerminalNode *> GreaterEqual();
    antlr4::tree::TerminalNode* GreaterEqual(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RelationalExpressionContext* relationalExpression();

  class  ShiftExpressionContext : public antlr4::ParserRuleContext {
  public:
    ShiftExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AdditiveExpressionContext *additiveExpression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ShiftExpressionContext* shiftExpression();

  class  AdditiveExpressionContext : public antlr4::ParserRuleContext {
  public:
    AdditiveExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<MultiplicativeExpressionContext *> multiplicativeExpression();
    MultiplicativeExpressionContext* multiplicativeExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Plus();
    antlr4::tree::TerminalNode* Plus(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Minus();
    antlr4::tree::TerminalNode* Minus(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AdditiveExpressionContext* additiveExpression();

  class  MultiplicativeExpressionContext : public antlr4::ParserRuleContext {
  public:
    MultiplicativeExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<CastExpressionContext *> castExpression();
    CastExpressionContext* castExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Star();
    antlr4::tree::TerminalNode* Star(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Slash();
    antlr4::tree::TerminalNode* Slash(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Percent();
    antlr4::tree::TerminalNode* Percent(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MultiplicativeExpressionContext* multiplicativeExpression();

  class  CastExpressionContext : public antlr4::ParserRuleContext {
  public:
    CastExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    UnaryExpressionContext *unaryExpression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CastExpressionContext* castExpression();

  class  UnaryExpressionContext : public antlr4::ParserRuleContext {
  public:
    UnaryExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PostfixExpressionContext *postfixExpression();
    antlr4::tree::TerminalNode *PlusPlus();
    UnaryExpressionContext *unaryExpression();
    antlr4::tree::TerminalNode *MinusMinus();
    UnaryOperatorContext *unaryOperator();
    CastExpressionContext *castExpression();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnaryExpressionContext* unaryExpression();

  class  UnaryOperatorContext : public antlr4::ParserRuleContext {
  public:
    UnaryOperatorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *And();
    antlr4::tree::TerminalNode *Star();
    antlr4::tree::TerminalNode *Plus();
    antlr4::tree::TerminalNode *Minus();
    antlr4::tree::TerminalNode *Tilde();
    antlr4::tree::TerminalNode *Not();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnaryOperatorContext* unaryOperator();

  class  PostfixExpressionContext : public antlr4::ParserRuleContext {
  public:
    PostfixExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    PrimaryExpressionContext *primaryExpression();
    std::vector<antlr4::tree::TerminalNode *> LeftParen();
    antlr4::tree::TerminalNode* LeftParen(size_t i);
    std::vector<antlr4::tree::TerminalNode *> RightParen();
    antlr4::tree::TerminalNode* RightParen(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PlusPlus();
    antlr4::tree::TerminalNode* PlusPlus(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MinusMinus();
    antlr4::tree::TerminalNode* MinusMinus(size_t i);
    std::vector<ArgumentExpressionListContext *> argumentExpressionList();
    ArgumentExpressionListContext* argumentExpressionList(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PostfixExpressionContext* postfixExpression();

  class  PrimaryExpressionContext : public antlr4::ParserRuleContext {
  public:
    PrimaryExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Identifier();
    ConstantContext *constant();
    antlr4::tree::TerminalNode *StringLiteral();
    antlr4::tree::TerminalNode *LeftParen();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RightParen();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrimaryExpressionContext* primaryExpression();

  class  ArgumentExpressionListContext : public antlr4::ParserRuleContext {
  public:
    ArgumentExpressionListContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AssignmentExpressionContext *> assignmentExpression();
    AssignmentExpressionContext* assignmentExpression(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Comma();
    antlr4::tree::TerminalNode* Comma(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ArgumentExpressionListContext* argumentExpressionList();

  class  ConstantContext : public antlr4::ParserRuleContext {
  public:
    ConstantContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *IntegerConstant();
    antlr4::tree::TerminalNode *FloatingConstant();
    antlr4::tree::TerminalNode *CharacterConstant();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstantContext* constant();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool directDeclaratorSempred(DirectDeclaratorContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
  /* private parser declarations section */
};

}  // namespace antlrcpptest
