/* parser/listener/visitor header section */

// Generated from ANTLR/TParser.g4 by ANTLR 4.13.2

/* parser precinclude section */

#include "TParserListener.h"
#include "TParserVisitor.h"

#include "TParser.h"


/* parser postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


using namespace antlrcpp;
using namespace antlrcpptest;

using namespace antlr4;

namespace {

struct TParserStaticData final {
  TParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  TParserStaticData(const TParserStaticData&) = delete;
  TParserStaticData(TParserStaticData&&) = delete;
  TParserStaticData& operator=(const TParserStaticData&) = delete;
  TParserStaticData& operator=(TParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag tparserParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<TParserStaticData> tparserParserStaticData = nullptr;

void tparserParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (tparserParserStaticData != nullptr) {
    return;
  }
#else
  assert(tparserParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<TParserStaticData>(
    std::vector<std::string>{
      "translationUnit", "externalDeclaration", "functionDefinition", "declaration", 
      "declarationSpecifiers", "declarationSpecifier", "storageClassSpecifier", 
      "typeSpecifier", "typeQualifier", "structSpecifier", "unionSpecifier", 
      "structDeclarationList", "structDeclaration", "structDeclaratorList", 
      "structDeclarator", "enumSpecifier", "enumeratorList", "enumerator", 
      "initDeclaratorList", "initDeclarator", "declarator", "directDeclarator", 
      "pointer", "parameterTypeList", "parameterList", "parameterDeclaration", 
      "initializer", "initializerList", "statement", "labeledStatement", 
      "compoundStatement", "blockItemList", "blockItem", "expressionStatement", 
      "selectionStatement", "iterationStatement", "jumpStatement", "expression", 
      "assignmentExpression", "conditionalExpression", "constantExpression", 
      "logicalOrExpression", "logicalAndExpression", "inclusiveOrExpression", 
      "exclusiveOrExpression", "andExpression", "equalityExpression", "relationalExpression", 
      "shiftExpression", "additiveExpression", "multiplicativeExpression", 
      "castExpression", "unaryExpression", "unaryOperator", "postfixExpression", 
      "primaryExpression", "argumentExpressionList", "constant"
    },
    std::vector<std::string>{
      "", "", "'auto'", "'break'", "'case'", "'char'", "'const'", "'continue'", 
      "'default'", "'do'", "'double'", "'else'", "'enum'", "'extern'", "'float'", 
      "'for'", "'goto'", "'if'", "'inline'", "'int'", "'long'", "'register'", 
      "'restrict'", "'return'", "'short'", "'signed'", "'sizeof'", "'static'", 
      "'struct'", "'switch'", "'typedef'", "'union'", "'unsigned'", "'void'", 
      "'volatile'", "'while'", "'_Alignas'", "'_Alignof'", "'_Atomic'", 
      "'_Bool'", "'_Complex'", "'_Generic'", "'_Imaginary'", "'_Noreturn'", 
      "'_Static_assert'", "'_Thread_local'", "", "", "", "", "", "", "", 
      "'('", "')'", "'['", "']'", "'{'", "'}'", "'.'", "'->'", "'++'", "'--'", 
      "'+'", "'-'", "'*'", "'/'", "'%'", "'&'", "'|'", "'^'", "'~'", "'!'", 
      "'='", "'<'", "'>'", "'<='", "'>='", "'=='", "'!='", "'&&'", "'||'", 
      "'\\u003F'", "':'", "';'", "','", "'...'"
    },
    std::vector<std::string>{
      "", "DUMMY", "Auto", "Break", "Case", "Char", "Const", "Continue", 
      "Default", "Do", "Double", "Else", "Enum", "Extern", "Float", "For", 
      "Goto", "If", "Inline", "Int", "Long", "Register", "Restrict", "Return", 
      "Short", "Signed", "Sizeof", "Static", "Struct", "Switch", "Typedef", 
      "Union", "Unsigned", "Void", "Volatile", "While", "Alignas", "Alignof", 
      "Atomic", "Bool", "Complex", "Generic", "Imaginary", "Noreturn", "StaticAssert", 
      "ThreadLocal", "CxxClass", "CxxNamespace", "Identifier", "IntegerConstant", 
      "FloatingConstant", "CharacterConstant", "StringLiteral", "LeftParen", 
      "RightParen", "LeftBracket", "RightBracket", "LeftBrace", "RightBrace", 
      "Dot", "Arrow", "PlusPlus", "MinusMinus", "Plus", "Minus", "Star", 
      "Slash", "Percent", "And", "Or", "Caret", "Tilde", "Not", "Assign", 
      "Less", "Greater", "LessEqual", "GreaterEqual", "Equal", "NotEqual", 
      "AndAnd", "OrOr", "Question", "Colon", "Semi", "Comma", "Ellipsis", 
      "Preprocessor", "LineComment", "BlockComment", "Whitespace"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,90,546,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,7,
  	28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,7,
  	35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,7,
  	42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,7,
  	49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,7,
  	56,2,57,7,57,1,0,5,0,118,8,0,10,0,12,0,121,9,0,1,1,1,1,3,1,125,8,1,1,
  	2,1,2,1,2,1,2,1,3,1,3,3,3,133,8,3,1,3,1,3,1,4,4,4,138,8,4,11,4,12,4,139,
  	1,5,1,5,1,5,3,5,145,8,5,1,6,1,6,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,1,
  	7,1,7,1,7,1,7,3,7,162,8,7,1,8,1,8,1,9,1,9,3,9,168,8,9,1,9,1,9,1,9,1,9,
  	1,10,1,10,3,10,176,8,10,1,10,1,10,1,10,1,10,1,11,4,11,183,8,11,11,11,
  	12,11,184,1,12,1,12,1,12,1,12,1,13,1,13,1,13,5,13,194,8,13,10,13,12,13,
  	197,9,13,1,14,1,14,1,15,1,15,3,15,203,8,15,1,15,1,15,1,15,1,15,1,16,1,
  	16,1,16,5,16,212,8,16,10,16,12,16,215,9,16,1,16,3,16,218,8,16,1,17,1,
  	17,1,17,3,17,223,8,17,1,18,1,18,1,18,5,18,228,8,18,10,18,12,18,231,9,
  	18,1,19,1,19,1,19,3,19,236,8,19,1,20,3,20,239,8,20,1,20,1,20,1,21,1,21,
  	1,21,1,21,1,21,1,21,3,21,249,8,21,1,21,1,21,1,21,3,21,254,8,21,1,21,5,
  	21,257,8,21,10,21,12,21,260,9,21,1,22,1,22,5,22,264,8,22,10,22,12,22,
  	267,9,22,1,22,3,22,270,8,22,1,23,1,23,1,24,1,24,1,24,5,24,277,8,24,10,
  	24,12,24,280,9,24,1,25,1,25,1,25,1,26,1,26,1,26,1,26,1,26,3,26,290,8,
  	26,1,27,1,27,1,27,5,27,295,8,27,10,27,12,27,298,9,27,1,27,3,27,301,8,
  	27,1,28,1,28,1,28,1,28,1,28,1,28,3,28,309,8,28,1,29,1,29,1,29,1,29,1,
  	29,1,29,1,29,1,29,1,29,1,29,1,29,3,29,322,8,29,1,30,1,30,3,30,326,8,30,
  	1,30,1,30,1,31,4,31,331,8,31,11,31,12,31,332,1,32,1,32,3,32,337,8,32,
  	1,33,3,33,340,8,33,1,33,1,33,1,34,1,34,1,34,1,34,1,34,1,34,1,34,3,34,
  	351,8,34,1,34,1,34,1,34,1,34,1,34,1,34,3,34,359,8,34,1,35,1,35,1,35,1,
  	35,1,35,1,35,1,35,1,35,1,35,1,35,1,35,1,35,1,35,1,35,1,35,1,35,1,35,1,
  	35,1,35,3,35,380,8,35,1,35,1,35,1,35,3,35,385,8,35,1,36,1,36,1,36,1,36,
  	1,36,1,36,1,36,1,36,1,36,3,36,396,8,36,1,36,3,36,399,8,36,1,37,1,37,1,
  	37,5,37,404,8,37,10,37,12,37,407,9,37,1,38,1,38,1,38,1,38,1,38,3,38,414,
  	8,38,1,39,1,39,1,39,1,39,1,39,1,39,3,39,422,8,39,1,40,1,40,1,41,1,41,
  	1,41,5,41,429,8,41,10,41,12,41,432,9,41,1,42,1,42,1,42,5,42,437,8,42,
  	10,42,12,42,440,9,42,1,43,1,43,1,43,5,43,445,8,43,10,43,12,43,448,9,43,
  	1,44,1,44,1,44,5,44,453,8,44,10,44,12,44,456,9,44,1,45,1,45,1,45,5,45,
  	461,8,45,10,45,12,45,464,9,45,1,46,1,46,1,46,5,46,469,8,46,10,46,12,46,
  	472,9,46,1,47,1,47,1,47,5,47,477,8,47,10,47,12,47,480,9,47,1,48,1,48,
  	1,49,1,49,1,49,5,49,487,8,49,10,49,12,49,490,9,49,1,50,1,50,1,50,5,50,
  	495,8,50,10,50,12,50,498,9,50,1,51,1,51,1,52,1,52,1,52,1,52,1,52,1,52,
  	1,52,1,52,3,52,510,8,52,1,53,1,53,1,54,1,54,1,54,3,54,517,8,54,1,54,1,
  	54,1,54,5,54,522,8,54,10,54,12,54,525,9,54,1,55,1,55,1,55,1,55,1,55,1,
  	55,1,55,3,55,534,8,55,1,56,1,56,1,56,5,56,539,8,56,10,56,12,56,542,9,
  	56,1,57,1,57,1,57,0,1,42,58,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,
  	32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,
  	78,80,82,84,86,88,90,92,94,96,98,100,102,104,106,108,110,112,114,0,8,
  	5,0,2,2,13,13,21,21,27,27,30,30,4,0,6,6,22,22,34,34,38,38,1,0,78,79,1,
  	0,74,77,1,0,63,64,1,0,65,67,3,0,63,65,68,68,71,72,1,0,49,51,568,0,119,
  	1,0,0,0,2,124,1,0,0,0,4,126,1,0,0,0,6,130,1,0,0,0,8,137,1,0,0,0,10,144,
  	1,0,0,0,12,146,1,0,0,0,14,161,1,0,0,0,16,163,1,0,0,0,18,165,1,0,0,0,20,
  	173,1,0,0,0,22,182,1,0,0,0,24,186,1,0,0,0,26,190,1,0,0,0,28,198,1,0,0,
  	0,30,200,1,0,0,0,32,208,1,0,0,0,34,219,1,0,0,0,36,224,1,0,0,0,38,232,
  	1,0,0,0,40,238,1,0,0,0,42,248,1,0,0,0,44,261,1,0,0,0,46,271,1,0,0,0,48,
  	273,1,0,0,0,50,281,1,0,0,0,52,289,1,0,0,0,54,291,1,0,0,0,56,308,1,0,0,
  	0,58,321,1,0,0,0,60,323,1,0,0,0,62,330,1,0,0,0,64,336,1,0,0,0,66,339,
  	1,0,0,0,68,358,1,0,0,0,70,384,1,0,0,0,72,398,1,0,0,0,74,400,1,0,0,0,76,
  	413,1,0,0,0,78,415,1,0,0,0,80,423,1,0,0,0,82,425,1,0,0,0,84,433,1,0,0,
  	0,86,441,1,0,0,0,88,449,1,0,0,0,90,457,1,0,0,0,92,465,1,0,0,0,94,473,
  	1,0,0,0,96,481,1,0,0,0,98,483,1,0,0,0,100,491,1,0,0,0,102,499,1,0,0,0,
  	104,509,1,0,0,0,106,511,1,0,0,0,108,513,1,0,0,0,110,533,1,0,0,0,112,535,
  	1,0,0,0,114,543,1,0,0,0,116,118,3,2,1,0,117,116,1,0,0,0,118,121,1,0,0,
  	0,119,117,1,0,0,0,119,120,1,0,0,0,120,1,1,0,0,0,121,119,1,0,0,0,122,125,
  	3,4,2,0,123,125,3,6,3,0,124,122,1,0,0,0,124,123,1,0,0,0,125,3,1,0,0,0,
  	126,127,3,8,4,0,127,128,3,40,20,0,128,129,3,60,30,0,129,5,1,0,0,0,130,
  	132,3,8,4,0,131,133,3,36,18,0,132,131,1,0,0,0,132,133,1,0,0,0,133,134,
  	1,0,0,0,134,135,5,84,0,0,135,7,1,0,0,0,136,138,3,10,5,0,137,136,1,0,0,
  	0,138,139,1,0,0,0,139,137,1,0,0,0,139,140,1,0,0,0,140,9,1,0,0,0,141,145,
  	3,12,6,0,142,145,3,14,7,0,143,145,3,16,8,0,144,141,1,0,0,0,144,142,1,
  	0,0,0,144,143,1,0,0,0,145,11,1,0,0,0,146,147,7,0,0,0,147,13,1,0,0,0,148,
  	162,5,33,0,0,149,162,5,5,0,0,150,162,5,24,0,0,151,162,5,19,0,0,152,162,
  	5,20,0,0,153,162,5,14,0,0,154,162,5,10,0,0,155,162,5,25,0,0,156,162,5,
  	32,0,0,157,162,5,39,0,0,158,162,3,18,9,0,159,162,3,20,10,0,160,162,3,
  	30,15,0,161,148,1,0,0,0,161,149,1,0,0,0,161,150,1,0,0,0,161,151,1,0,0,
  	0,161,152,1,0,0,0,161,153,1,0,0,0,161,154,1,0,0,0,161,155,1,0,0,0,161,
  	156,1,0,0,0,161,157,1,0,0,0,161,158,1,0,0,0,161,159,1,0,0,0,161,160,1,
  	0,0,0,162,15,1,0,0,0,163,164,7,1,0,0,164,17,1,0,0,0,165,167,5,28,0,0,
  	166,168,5,48,0,0,167,166,1,0,0,0,167,168,1,0,0,0,168,169,1,0,0,0,169,
  	170,5,57,0,0,170,171,3,22,11,0,171,172,5,58,0,0,172,19,1,0,0,0,173,175,
  	5,31,0,0,174,176,5,48,0,0,175,174,1,0,0,0,175,176,1,0,0,0,176,177,1,0,
  	0,0,177,178,5,57,0,0,178,179,3,22,11,0,179,180,5,58,0,0,180,21,1,0,0,
  	0,181,183,3,24,12,0,182,181,1,0,0,0,183,184,1,0,0,0,184,182,1,0,0,0,184,
  	185,1,0,0,0,185,23,1,0,0,0,186,187,3,8,4,0,187,188,3,26,13,0,188,189,
  	5,84,0,0,189,25,1,0,0,0,190,195,3,28,14,0,191,192,5,85,0,0,192,194,3,
  	28,14,0,193,191,1,0,0,0,194,197,1,0,0,0,195,193,1,0,0,0,195,196,1,0,0,
  	0,196,27,1,0,0,0,197,195,1,0,0,0,198,199,3,40,20,0,199,29,1,0,0,0,200,
  	202,5,12,0,0,201,203,5,48,0,0,202,201,1,0,0,0,202,203,1,0,0,0,203,204,
  	1,0,0,0,204,205,5,57,0,0,205,206,3,32,16,0,206,207,5,58,0,0,207,31,1,
  	0,0,0,208,213,3,34,17,0,209,210,5,85,0,0,210,212,3,34,17,0,211,209,1,
  	0,0,0,212,215,1,0,0,0,213,211,1,0,0,0,213,214,1,0,0,0,214,217,1,0,0,0,
  	215,213,1,0,0,0,216,218,5,85,0,0,217,216,1,0,0,0,217,218,1,0,0,0,218,
  	33,1,0,0,0,219,222,5,48,0,0,220,221,5,73,0,0,221,223,3,80,40,0,222,220,
  	1,0,0,0,222,223,1,0,0,0,223,35,1,0,0,0,224,229,3,38,19,0,225,226,5,85,
  	0,0,226,228,3,38,19,0,227,225,1,0,0,0,228,231,1,0,0,0,229,227,1,0,0,0,
  	229,230,1,0,0,0,230,37,1,0,0,0,231,229,1,0,0,0,232,235,3,40,20,0,233,
  	234,5,73,0,0,234,236,3,52,26,0,235,233,1,0,0,0,235,236,1,0,0,0,236,39,
  	1,0,0,0,237,239,3,44,22,0,238,237,1,0,0,0,238,239,1,0,0,0,239,240,1,0,
  	0,0,240,241,3,42,21,0,241,41,1,0,0,0,242,243,6,21,-1,0,243,249,5,48,0,
  	0,244,245,5,53,0,0,245,246,3,40,20,0,246,247,5,54,0,0,247,249,1,0,0,0,
  	248,242,1,0,0,0,248,244,1,0,0,0,249,258,1,0,0,0,250,251,10,1,0,0,251,
  	253,5,53,0,0,252,254,3,46,23,0,253,252,1,0,0,0,253,254,1,0,0,0,254,255,
  	1,0,0,0,255,257,5,54,0,0,256,250,1,0,0,0,257,260,1,0,0,0,258,256,1,0,
  	0,0,258,259,1,0,0,0,259,43,1,0,0,0,260,258,1,0,0,0,261,265,5,65,0,0,262,
  	264,3,16,8,0,263,262,1,0,0,0,264,267,1,0,0,0,265,263,1,0,0,0,265,266,
  	1,0,0,0,266,269,1,0,0,0,267,265,1,0,0,0,268,270,3,44,22,0,269,268,1,0,
  	0,0,269,270,1,0,0,0,270,45,1,0,0,0,271,272,3,48,24,0,272,47,1,0,0,0,273,
  	278,3,50,25,0,274,275,5,85,0,0,275,277,3,50,25,0,276,274,1,0,0,0,277,
  	280,1,0,0,0,278,276,1,0,0,0,278,279,1,0,0,0,279,49,1,0,0,0,280,278,1,
  	0,0,0,281,282,3,8,4,0,282,283,3,40,20,0,283,51,1,0,0,0,284,290,3,76,38,
  	0,285,286,5,57,0,0,286,287,3,54,27,0,287,288,5,58,0,0,288,290,1,0,0,0,
  	289,284,1,0,0,0,289,285,1,0,0,0,290,53,1,0,0,0,291,296,3,52,26,0,292,
  	293,5,85,0,0,293,295,3,52,26,0,294,292,1,0,0,0,295,298,1,0,0,0,296,294,
  	1,0,0,0,296,297,1,0,0,0,297,300,1,0,0,0,298,296,1,0,0,0,299,301,5,85,
  	0,0,300,299,1,0,0,0,300,301,1,0,0,0,301,55,1,0,0,0,302,309,3,58,29,0,
  	303,309,3,60,30,0,304,309,3,66,33,0,305,309,3,68,34,0,306,309,3,70,35,
  	0,307,309,3,72,36,0,308,302,1,0,0,0,308,303,1,0,0,0,308,304,1,0,0,0,308,
  	305,1,0,0,0,308,306,1,0,0,0,308,307,1,0,0,0,309,57,1,0,0,0,310,311,5,
  	48,0,0,311,312,5,83,0,0,312,322,3,56,28,0,313,314,5,4,0,0,314,315,3,80,
  	40,0,315,316,5,83,0,0,316,317,3,56,28,0,317,322,1,0,0,0,318,319,5,8,0,
  	0,319,320,5,83,0,0,320,322,3,56,28,0,321,310,1,0,0,0,321,313,1,0,0,0,
  	321,318,1,0,0,0,322,59,1,0,0,0,323,325,5,57,0,0,324,326,3,62,31,0,325,
  	324,1,0,0,0,325,326,1,0,0,0,326,327,1,0,0,0,327,328,5,58,0,0,328,61,1,
  	0,0,0,329,331,3,64,32,0,330,329,1,0,0,0,331,332,1,0,0,0,332,330,1,0,0,
  	0,332,333,1,0,0,0,333,63,1,0,0,0,334,337,3,6,3,0,335,337,3,56,28,0,336,
  	334,1,0,0,0,336,335,1,0,0,0,337,65,1,0,0,0,338,340,3,74,37,0,339,338,
  	1,0,0,0,339,340,1,0,0,0,340,341,1,0,0,0,341,342,5,84,0,0,342,67,1,0,0,
  	0,343,344,5,17,0,0,344,345,5,53,0,0,345,346,3,74,37,0,346,347,5,54,0,
  	0,347,350,3,56,28,0,348,349,5,11,0,0,349,351,3,56,28,0,350,348,1,0,0,
  	0,350,351,1,0,0,0,351,359,1,0,0,0,352,353,5,29,0,0,353,354,5,53,0,0,354,
  	355,3,74,37,0,355,356,5,54,0,0,356,357,3,56,28,0,357,359,1,0,0,0,358,
  	343,1,0,0,0,358,352,1,0,0,0,359,69,1,0,0,0,360,361,5,35,0,0,361,362,5,
  	53,0,0,362,363,3,74,37,0,363,364,5,54,0,0,364,365,3,56,28,0,365,385,1,
  	0,0,0,366,367,5,9,0,0,367,368,3,56,28,0,368,369,5,35,0,0,369,370,5,53,
  	0,0,370,371,3,74,37,0,371,372,5,54,0,0,372,373,5,84,0,0,373,385,1,0,0,
  	0,374,375,5,15,0,0,375,376,5,53,0,0,376,377,3,66,33,0,377,379,3,66,33,
  	0,378,380,3,74,37,0,379,378,1,0,0,0,379,380,1,0,0,0,380,381,1,0,0,0,381,
  	382,5,54,0,0,382,383,3,56,28,0,383,385,1,0,0,0,384,360,1,0,0,0,384,366,
  	1,0,0,0,384,374,1,0,0,0,385,71,1,0,0,0,386,387,5,16,0,0,387,388,5,48,
  	0,0,388,399,5,84,0,0,389,390,5,7,0,0,390,399,5,84,0,0,391,392,5,3,0,0,
  	392,399,5,84,0,0,393,395,5,23,0,0,394,396,3,74,37,0,395,394,1,0,0,0,395,
  	396,1,0,0,0,396,397,1,0,0,0,397,399,5,84,0,0,398,386,1,0,0,0,398,389,
  	1,0,0,0,398,391,1,0,0,0,398,393,1,0,0,0,399,73,1,0,0,0,400,405,3,76,38,
  	0,401,402,5,85,0,0,402,404,3,76,38,0,403,401,1,0,0,0,404,407,1,0,0,0,
  	405,403,1,0,0,0,405,406,1,0,0,0,406,75,1,0,0,0,407,405,1,0,0,0,408,414,
  	3,78,39,0,409,410,3,104,52,0,410,411,5,73,0,0,411,412,3,76,38,0,412,414,
  	1,0,0,0,413,408,1,0,0,0,413,409,1,0,0,0,414,77,1,0,0,0,415,421,3,82,41,
  	0,416,417,5,82,0,0,417,418,3,74,37,0,418,419,5,83,0,0,419,420,3,78,39,
  	0,420,422,1,0,0,0,421,416,1,0,0,0,421,422,1,0,0,0,422,79,1,0,0,0,423,
  	424,3,78,39,0,424,81,1,0,0,0,425,430,3,84,42,0,426,427,5,81,0,0,427,429,
  	3,84,42,0,428,426,1,0,0,0,429,432,1,0,0,0,430,428,1,0,0,0,430,431,1,0,
  	0,0,431,83,1,0,0,0,432,430,1,0,0,0,433,438,3,86,43,0,434,435,5,80,0,0,
  	435,437,3,86,43,0,436,434,1,0,0,0,437,440,1,0,0,0,438,436,1,0,0,0,438,
  	439,1,0,0,0,439,85,1,0,0,0,440,438,1,0,0,0,441,446,3,88,44,0,442,443,
  	5,69,0,0,443,445,3,88,44,0,444,442,1,0,0,0,445,448,1,0,0,0,446,444,1,
  	0,0,0,446,447,1,0,0,0,447,87,1,0,0,0,448,446,1,0,0,0,449,454,3,90,45,
  	0,450,451,5,70,0,0,451,453,3,90,45,0,452,450,1,0,0,0,453,456,1,0,0,0,
  	454,452,1,0,0,0,454,455,1,0,0,0,455,89,1,0,0,0,456,454,1,0,0,0,457,462,
  	3,92,46,0,458,459,5,68,0,0,459,461,3,92,46,0,460,458,1,0,0,0,461,464,
  	1,0,0,0,462,460,1,0,0,0,462,463,1,0,0,0,463,91,1,0,0,0,464,462,1,0,0,
  	0,465,470,3,94,47,0,466,467,7,2,0,0,467,469,3,94,47,0,468,466,1,0,0,0,
  	469,472,1,0,0,0,470,468,1,0,0,0,470,471,1,0,0,0,471,93,1,0,0,0,472,470,
  	1,0,0,0,473,478,3,96,48,0,474,475,7,3,0,0,475,477,3,96,48,0,476,474,1,
  	0,0,0,477,480,1,0,0,0,478,476,1,0,0,0,478,479,1,0,0,0,479,95,1,0,0,0,
  	480,478,1,0,0,0,481,482,3,98,49,0,482,97,1,0,0,0,483,488,3,100,50,0,484,
  	485,7,4,0,0,485,487,3,100,50,0,486,484,1,0,0,0,487,490,1,0,0,0,488,486,
  	1,0,0,0,488,489,1,0,0,0,489,99,1,0,0,0,490,488,1,0,0,0,491,496,3,102,
  	51,0,492,493,7,5,0,0,493,495,3,102,51,0,494,492,1,0,0,0,495,498,1,0,0,
  	0,496,494,1,0,0,0,496,497,1,0,0,0,497,101,1,0,0,0,498,496,1,0,0,0,499,
  	500,3,104,52,0,500,103,1,0,0,0,501,510,3,108,54,0,502,503,5,61,0,0,503,
  	510,3,104,52,0,504,505,5,62,0,0,505,510,3,104,52,0,506,507,3,106,53,0,
  	507,508,3,102,51,0,508,510,1,0,0,0,509,501,1,0,0,0,509,502,1,0,0,0,509,
  	504,1,0,0,0,509,506,1,0,0,0,510,105,1,0,0,0,511,512,7,6,0,0,512,107,1,
  	0,0,0,513,523,3,110,55,0,514,516,5,53,0,0,515,517,3,112,56,0,516,515,
  	1,0,0,0,516,517,1,0,0,0,517,518,1,0,0,0,518,522,5,54,0,0,519,522,5,61,
  	0,0,520,522,5,62,0,0,521,514,1,0,0,0,521,519,1,0,0,0,521,520,1,0,0,0,
  	522,525,1,0,0,0,523,521,1,0,0,0,523,524,1,0,0,0,524,109,1,0,0,0,525,523,
  	1,0,0,0,526,534,5,48,0,0,527,534,3,114,57,0,528,534,5,52,0,0,529,530,
  	5,53,0,0,530,531,3,74,37,0,531,532,5,54,0,0,532,534,1,0,0,0,533,526,1,
  	0,0,0,533,527,1,0,0,0,533,528,1,0,0,0,533,529,1,0,0,0,534,111,1,0,0,0,
  	535,540,3,76,38,0,536,537,5,85,0,0,537,539,3,76,38,0,538,536,1,0,0,0,
  	539,542,1,0,0,0,540,538,1,0,0,0,540,541,1,0,0,0,541,113,1,0,0,0,542,540,
  	1,0,0,0,543,544,7,7,0,0,544,115,1,0,0,0,56,119,124,132,139,144,161,167,
  	175,184,195,202,213,217,222,229,235,238,248,253,258,265,269,278,289,296,
  	300,308,321,325,332,336,339,350,358,379,384,395,398,405,413,421,430,438,
  	446,454,462,470,478,488,496,509,516,521,523,533,540
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  tparserParserStaticData = std::move(staticData);
}

}

TParser::TParser(TokenStream *input) : TParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

TParser::TParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  TParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *tparserParserStaticData->atn, tparserParserStaticData->decisionToDFA, tparserParserStaticData->sharedContextCache, options);
}

TParser::~TParser() {
  delete _interpreter;
}

const atn::ATN& TParser::getATN() const {
  return *tparserParserStaticData->atn;
}

std::string TParser::getGrammarFileName() const {
  return "TParser.g4";
}

const std::vector<std::string>& TParser::getRuleNames() const {
  return tparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& TParser::getVocabulary() const {
  return tparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView TParser::getSerializedATN() const {
  return tparserParserStaticData->serializedATN;
}

/* parser definitions section */

//----------------- TranslationUnitContext ------------------------------------------------------------------

TParser::TranslationUnitContext::TranslationUnitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::ExternalDeclarationContext *> TParser::TranslationUnitContext::externalDeclaration() {
  return getRuleContexts<TParser::ExternalDeclarationContext>();
}

TParser::ExternalDeclarationContext* TParser::TranslationUnitContext::externalDeclaration(size_t i) {
  return getRuleContext<TParser::ExternalDeclarationContext>(i);
}


size_t TParser::TranslationUnitContext::getRuleIndex() const {
  return TParser::RuleTranslationUnit;
}

void TParser::TranslationUnitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTranslationUnit(this);
}

void TParser::TranslationUnitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTranslationUnit(this);
}


std::any TParser::TranslationUnitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitTranslationUnit(this);
  else
    return visitor->visitChildren(this);
}

TParser::TranslationUnitContext* TParser::translationUnit() {
  TranslationUnitContext *_localctx = _tracker.createInstance<TranslationUnitContext>(_ctx, getState());
  enterRule(_localctx, 0, TParser::RuleTranslationUnit);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(119);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 858380596324) != 0)) {
      setState(116);
      externalDeclaration();
      setState(121);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExternalDeclarationContext ------------------------------------------------------------------

TParser::ExternalDeclarationContext::ExternalDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::FunctionDefinitionContext* TParser::ExternalDeclarationContext::functionDefinition() {
  return getRuleContext<TParser::FunctionDefinitionContext>(0);
}

TParser::DeclarationContext* TParser::ExternalDeclarationContext::declaration() {
  return getRuleContext<TParser::DeclarationContext>(0);
}


size_t TParser::ExternalDeclarationContext::getRuleIndex() const {
  return TParser::RuleExternalDeclaration;
}

void TParser::ExternalDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExternalDeclaration(this);
}

void TParser::ExternalDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExternalDeclaration(this);
}


std::any TParser::ExternalDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitExternalDeclaration(this);
  else
    return visitor->visitChildren(this);
}

TParser::ExternalDeclarationContext* TParser::externalDeclaration() {
  ExternalDeclarationContext *_localctx = _tracker.createInstance<ExternalDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 2, TParser::RuleExternalDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(124);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(122);
      functionDefinition();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(123);
      declaration();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDefinitionContext ------------------------------------------------------------------

TParser::FunctionDefinitionContext::FunctionDefinitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::DeclarationSpecifiersContext* TParser::FunctionDefinitionContext::declarationSpecifiers() {
  return getRuleContext<TParser::DeclarationSpecifiersContext>(0);
}

TParser::DeclaratorContext* TParser::FunctionDefinitionContext::declarator() {
  return getRuleContext<TParser::DeclaratorContext>(0);
}

TParser::CompoundStatementContext* TParser::FunctionDefinitionContext::compoundStatement() {
  return getRuleContext<TParser::CompoundStatementContext>(0);
}


size_t TParser::FunctionDefinitionContext::getRuleIndex() const {
  return TParser::RuleFunctionDefinition;
}

void TParser::FunctionDefinitionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionDefinition(this);
}

void TParser::FunctionDefinitionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionDefinition(this);
}


std::any TParser::FunctionDefinitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitFunctionDefinition(this);
  else
    return visitor->visitChildren(this);
}

TParser::FunctionDefinitionContext* TParser::functionDefinition() {
  FunctionDefinitionContext *_localctx = _tracker.createInstance<FunctionDefinitionContext>(_ctx, getState());
  enterRule(_localctx, 4, TParser::RuleFunctionDefinition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(126);
    declarationSpecifiers();
    setState(127);
    declarator();
    setState(128);
    compoundStatement();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationContext ------------------------------------------------------------------

TParser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::DeclarationSpecifiersContext* TParser::DeclarationContext::declarationSpecifiers() {
  return getRuleContext<TParser::DeclarationSpecifiersContext>(0);
}

tree::TerminalNode* TParser::DeclarationContext::Semi() {
  return getToken(TParser::Semi, 0);
}

TParser::InitDeclaratorListContext* TParser::DeclarationContext::initDeclaratorList() {
  return getRuleContext<TParser::InitDeclaratorListContext>(0);
}


size_t TParser::DeclarationContext::getRuleIndex() const {
  return TParser::RuleDeclaration;
}

void TParser::DeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclaration(this);
}

void TParser::DeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclaration(this);
}


std::any TParser::DeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

TParser::DeclarationContext* TParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 6, TParser::RuleDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(130);
    declarationSpecifiers();
    setState(132);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 48) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 48)) & 131105) != 0)) {
      setState(131);
      initDeclaratorList();
    }
    setState(134);
    match(TParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationSpecifiersContext ------------------------------------------------------------------

TParser::DeclarationSpecifiersContext::DeclarationSpecifiersContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::DeclarationSpecifierContext *> TParser::DeclarationSpecifiersContext::declarationSpecifier() {
  return getRuleContexts<TParser::DeclarationSpecifierContext>();
}

TParser::DeclarationSpecifierContext* TParser::DeclarationSpecifiersContext::declarationSpecifier(size_t i) {
  return getRuleContext<TParser::DeclarationSpecifierContext>(i);
}


size_t TParser::DeclarationSpecifiersContext::getRuleIndex() const {
  return TParser::RuleDeclarationSpecifiers;
}

void TParser::DeclarationSpecifiersContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclarationSpecifiers(this);
}

void TParser::DeclarationSpecifiersContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclarationSpecifiers(this);
}


std::any TParser::DeclarationSpecifiersContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitDeclarationSpecifiers(this);
  else
    return visitor->visitChildren(this);
}

TParser::DeclarationSpecifiersContext* TParser::declarationSpecifiers() {
  DeclarationSpecifiersContext *_localctx = _tracker.createInstance<DeclarationSpecifiersContext>(_ctx, getState());
  enterRule(_localctx, 8, TParser::RuleDeclarationSpecifiers);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(137); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(136);
      declarationSpecifier();
      setState(139); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 858380596324) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationSpecifierContext ------------------------------------------------------------------

TParser::DeclarationSpecifierContext::DeclarationSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::StorageClassSpecifierContext* TParser::DeclarationSpecifierContext::storageClassSpecifier() {
  return getRuleContext<TParser::StorageClassSpecifierContext>(0);
}

TParser::TypeSpecifierContext* TParser::DeclarationSpecifierContext::typeSpecifier() {
  return getRuleContext<TParser::TypeSpecifierContext>(0);
}

TParser::TypeQualifierContext* TParser::DeclarationSpecifierContext::typeQualifier() {
  return getRuleContext<TParser::TypeQualifierContext>(0);
}


size_t TParser::DeclarationSpecifierContext::getRuleIndex() const {
  return TParser::RuleDeclarationSpecifier;
}

void TParser::DeclarationSpecifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclarationSpecifier(this);
}

void TParser::DeclarationSpecifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclarationSpecifier(this);
}


std::any TParser::DeclarationSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitDeclarationSpecifier(this);
  else
    return visitor->visitChildren(this);
}

TParser::DeclarationSpecifierContext* TParser::declarationSpecifier() {
  DeclarationSpecifierContext *_localctx = _tracker.createInstance<DeclarationSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 10, TParser::RuleDeclarationSpecifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(144);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::Auto:
      case TParser::Extern:
      case TParser::Register:
      case TParser::Static:
      case TParser::Typedef: {
        enterOuterAlt(_localctx, 1);
        setState(141);
        storageClassSpecifier();
        break;
      }

      case TParser::Char:
      case TParser::Double:
      case TParser::Enum:
      case TParser::Float:
      case TParser::Int:
      case TParser::Long:
      case TParser::Short:
      case TParser::Signed:
      case TParser::Struct:
      case TParser::Union:
      case TParser::Unsigned:
      case TParser::Void:
      case TParser::Bool: {
        enterOuterAlt(_localctx, 2);
        setState(142);
        typeSpecifier();
        break;
      }

      case TParser::Const:
      case TParser::Restrict:
      case TParser::Volatile:
      case TParser::Atomic: {
        enterOuterAlt(_localctx, 3);
        setState(143);
        typeQualifier();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StorageClassSpecifierContext ------------------------------------------------------------------

TParser::StorageClassSpecifierContext::StorageClassSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::StorageClassSpecifierContext::Auto() {
  return getToken(TParser::Auto, 0);
}

tree::TerminalNode* TParser::StorageClassSpecifierContext::Register() {
  return getToken(TParser::Register, 0);
}

tree::TerminalNode* TParser::StorageClassSpecifierContext::Static() {
  return getToken(TParser::Static, 0);
}

tree::TerminalNode* TParser::StorageClassSpecifierContext::Extern() {
  return getToken(TParser::Extern, 0);
}

tree::TerminalNode* TParser::StorageClassSpecifierContext::Typedef() {
  return getToken(TParser::Typedef, 0);
}


size_t TParser::StorageClassSpecifierContext::getRuleIndex() const {
  return TParser::RuleStorageClassSpecifier;
}

void TParser::StorageClassSpecifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStorageClassSpecifier(this);
}

void TParser::StorageClassSpecifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStorageClassSpecifier(this);
}


std::any TParser::StorageClassSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitStorageClassSpecifier(this);
  else
    return visitor->visitChildren(this);
}

TParser::StorageClassSpecifierContext* TParser::storageClassSpecifier() {
  StorageClassSpecifierContext *_localctx = _tracker.createInstance<StorageClassSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 12, TParser::RuleStorageClassSpecifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(146);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1210064900) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeSpecifierContext ------------------------------------------------------------------

TParser::TypeSpecifierContext::TypeSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::TypeSpecifierContext::Void() {
  return getToken(TParser::Void, 0);
}

tree::TerminalNode* TParser::TypeSpecifierContext::Char() {
  return getToken(TParser::Char, 0);
}

tree::TerminalNode* TParser::TypeSpecifierContext::Short() {
  return getToken(TParser::Short, 0);
}

tree::TerminalNode* TParser::TypeSpecifierContext::Int() {
  return getToken(TParser::Int, 0);
}

tree::TerminalNode* TParser::TypeSpecifierContext::Long() {
  return getToken(TParser::Long, 0);
}

tree::TerminalNode* TParser::TypeSpecifierContext::Float() {
  return getToken(TParser::Float, 0);
}

tree::TerminalNode* TParser::TypeSpecifierContext::Double() {
  return getToken(TParser::Double, 0);
}

tree::TerminalNode* TParser::TypeSpecifierContext::Signed() {
  return getToken(TParser::Signed, 0);
}

tree::TerminalNode* TParser::TypeSpecifierContext::Unsigned() {
  return getToken(TParser::Unsigned, 0);
}

tree::TerminalNode* TParser::TypeSpecifierContext::Bool() {
  return getToken(TParser::Bool, 0);
}

TParser::StructSpecifierContext* TParser::TypeSpecifierContext::structSpecifier() {
  return getRuleContext<TParser::StructSpecifierContext>(0);
}

TParser::UnionSpecifierContext* TParser::TypeSpecifierContext::unionSpecifier() {
  return getRuleContext<TParser::UnionSpecifierContext>(0);
}

TParser::EnumSpecifierContext* TParser::TypeSpecifierContext::enumSpecifier() {
  return getRuleContext<TParser::EnumSpecifierContext>(0);
}


size_t TParser::TypeSpecifierContext::getRuleIndex() const {
  return TParser::RuleTypeSpecifier;
}

void TParser::TypeSpecifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeSpecifier(this);
}

void TParser::TypeSpecifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeSpecifier(this);
}


std::any TParser::TypeSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitTypeSpecifier(this);
  else
    return visitor->visitChildren(this);
}

TParser::TypeSpecifierContext* TParser::typeSpecifier() {
  TypeSpecifierContext *_localctx = _tracker.createInstance<TypeSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 14, TParser::RuleTypeSpecifier);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(161);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::Void: {
        enterOuterAlt(_localctx, 1);
        setState(148);
        match(TParser::Void);
        break;
      }

      case TParser::Char: {
        enterOuterAlt(_localctx, 2);
        setState(149);
        match(TParser::Char);
        break;
      }

      case TParser::Short: {
        enterOuterAlt(_localctx, 3);
        setState(150);
        match(TParser::Short);
        break;
      }

      case TParser::Int: {
        enterOuterAlt(_localctx, 4);
        setState(151);
        match(TParser::Int);
        break;
      }

      case TParser::Long: {
        enterOuterAlt(_localctx, 5);
        setState(152);
        match(TParser::Long);
        break;
      }

      case TParser::Float: {
        enterOuterAlt(_localctx, 6);
        setState(153);
        match(TParser::Float);
        break;
      }

      case TParser::Double: {
        enterOuterAlt(_localctx, 7);
        setState(154);
        match(TParser::Double);
        break;
      }

      case TParser::Signed: {
        enterOuterAlt(_localctx, 8);
        setState(155);
        match(TParser::Signed);
        break;
      }

      case TParser::Unsigned: {
        enterOuterAlt(_localctx, 9);
        setState(156);
        match(TParser::Unsigned);
        break;
      }

      case TParser::Bool: {
        enterOuterAlt(_localctx, 10);
        setState(157);
        match(TParser::Bool);
        break;
      }

      case TParser::Struct: {
        enterOuterAlt(_localctx, 11);
        setState(158);
        structSpecifier();
        break;
      }

      case TParser::Union: {
        enterOuterAlt(_localctx, 12);
        setState(159);
        unionSpecifier();
        break;
      }

      case TParser::Enum: {
        enterOuterAlt(_localctx, 13);
        setState(160);
        enumSpecifier();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeQualifierContext ------------------------------------------------------------------

TParser::TypeQualifierContext::TypeQualifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::TypeQualifierContext::Const() {
  return getToken(TParser::Const, 0);
}

tree::TerminalNode* TParser::TypeQualifierContext::Volatile() {
  return getToken(TParser::Volatile, 0);
}

tree::TerminalNode* TParser::TypeQualifierContext::Restrict() {
  return getToken(TParser::Restrict, 0);
}

tree::TerminalNode* TParser::TypeQualifierContext::Atomic() {
  return getToken(TParser::Atomic, 0);
}


size_t TParser::TypeQualifierContext::getRuleIndex() const {
  return TParser::RuleTypeQualifier;
}

void TParser::TypeQualifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeQualifier(this);
}

void TParser::TypeQualifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeQualifier(this);
}


std::any TParser::TypeQualifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitTypeQualifier(this);
  else
    return visitor->visitChildren(this);
}

TParser::TypeQualifierContext* TParser::typeQualifier() {
  TypeQualifierContext *_localctx = _tracker.createInstance<TypeQualifierContext>(_ctx, getState());
  enterRule(_localctx, 16, TParser::RuleTypeQualifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(163);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 292061970496) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructSpecifierContext ------------------------------------------------------------------

TParser::StructSpecifierContext::StructSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::StructSpecifierContext::Struct() {
  return getToken(TParser::Struct, 0);
}

tree::TerminalNode* TParser::StructSpecifierContext::LeftBrace() {
  return getToken(TParser::LeftBrace, 0);
}

TParser::StructDeclarationListContext* TParser::StructSpecifierContext::structDeclarationList() {
  return getRuleContext<TParser::StructDeclarationListContext>(0);
}

tree::TerminalNode* TParser::StructSpecifierContext::RightBrace() {
  return getToken(TParser::RightBrace, 0);
}

tree::TerminalNode* TParser::StructSpecifierContext::Identifier() {
  return getToken(TParser::Identifier, 0);
}


size_t TParser::StructSpecifierContext::getRuleIndex() const {
  return TParser::RuleStructSpecifier;
}

void TParser::StructSpecifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructSpecifier(this);
}

void TParser::StructSpecifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructSpecifier(this);
}


std::any TParser::StructSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitStructSpecifier(this);
  else
    return visitor->visitChildren(this);
}

TParser::StructSpecifierContext* TParser::structSpecifier() {
  StructSpecifierContext *_localctx = _tracker.createInstance<StructSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 18, TParser::RuleStructSpecifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(165);
    match(TParser::Struct);
    setState(167);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TParser::Identifier) {
      setState(166);
      match(TParser::Identifier);
    }
    setState(169);
    match(TParser::LeftBrace);
    setState(170);
    structDeclarationList();
    setState(171);
    match(TParser::RightBrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnionSpecifierContext ------------------------------------------------------------------

TParser::UnionSpecifierContext::UnionSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::UnionSpecifierContext::Union() {
  return getToken(TParser::Union, 0);
}

tree::TerminalNode* TParser::UnionSpecifierContext::LeftBrace() {
  return getToken(TParser::LeftBrace, 0);
}

TParser::StructDeclarationListContext* TParser::UnionSpecifierContext::structDeclarationList() {
  return getRuleContext<TParser::StructDeclarationListContext>(0);
}

tree::TerminalNode* TParser::UnionSpecifierContext::RightBrace() {
  return getToken(TParser::RightBrace, 0);
}

tree::TerminalNode* TParser::UnionSpecifierContext::Identifier() {
  return getToken(TParser::Identifier, 0);
}


size_t TParser::UnionSpecifierContext::getRuleIndex() const {
  return TParser::RuleUnionSpecifier;
}

void TParser::UnionSpecifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnionSpecifier(this);
}

void TParser::UnionSpecifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnionSpecifier(this);
}


std::any TParser::UnionSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitUnionSpecifier(this);
  else
    return visitor->visitChildren(this);
}

TParser::UnionSpecifierContext* TParser::unionSpecifier() {
  UnionSpecifierContext *_localctx = _tracker.createInstance<UnionSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 20, TParser::RuleUnionSpecifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(173);
    match(TParser::Union);
    setState(175);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TParser::Identifier) {
      setState(174);
      match(TParser::Identifier);
    }
    setState(177);
    match(TParser::LeftBrace);
    setState(178);
    structDeclarationList();
    setState(179);
    match(TParser::RightBrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructDeclarationListContext ------------------------------------------------------------------

TParser::StructDeclarationListContext::StructDeclarationListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::StructDeclarationContext *> TParser::StructDeclarationListContext::structDeclaration() {
  return getRuleContexts<TParser::StructDeclarationContext>();
}

TParser::StructDeclarationContext* TParser::StructDeclarationListContext::structDeclaration(size_t i) {
  return getRuleContext<TParser::StructDeclarationContext>(i);
}


size_t TParser::StructDeclarationListContext::getRuleIndex() const {
  return TParser::RuleStructDeclarationList;
}

void TParser::StructDeclarationListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructDeclarationList(this);
}

void TParser::StructDeclarationListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructDeclarationList(this);
}


std::any TParser::StructDeclarationListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitStructDeclarationList(this);
  else
    return visitor->visitChildren(this);
}

TParser::StructDeclarationListContext* TParser::structDeclarationList() {
  StructDeclarationListContext *_localctx = _tracker.createInstance<StructDeclarationListContext>(_ctx, getState());
  enterRule(_localctx, 22, TParser::RuleStructDeclarationList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(182); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(181);
      structDeclaration();
      setState(184); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 858380596324) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructDeclarationContext ------------------------------------------------------------------

TParser::StructDeclarationContext::StructDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::DeclarationSpecifiersContext* TParser::StructDeclarationContext::declarationSpecifiers() {
  return getRuleContext<TParser::DeclarationSpecifiersContext>(0);
}

TParser::StructDeclaratorListContext* TParser::StructDeclarationContext::structDeclaratorList() {
  return getRuleContext<TParser::StructDeclaratorListContext>(0);
}

tree::TerminalNode* TParser::StructDeclarationContext::Semi() {
  return getToken(TParser::Semi, 0);
}


size_t TParser::StructDeclarationContext::getRuleIndex() const {
  return TParser::RuleStructDeclaration;
}

void TParser::StructDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructDeclaration(this);
}

void TParser::StructDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructDeclaration(this);
}


std::any TParser::StructDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitStructDeclaration(this);
  else
    return visitor->visitChildren(this);
}

TParser::StructDeclarationContext* TParser::structDeclaration() {
  StructDeclarationContext *_localctx = _tracker.createInstance<StructDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 24, TParser::RuleStructDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(186);
    declarationSpecifiers();
    setState(187);
    structDeclaratorList();
    setState(188);
    match(TParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructDeclaratorListContext ------------------------------------------------------------------

TParser::StructDeclaratorListContext::StructDeclaratorListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::StructDeclaratorContext *> TParser::StructDeclaratorListContext::structDeclarator() {
  return getRuleContexts<TParser::StructDeclaratorContext>();
}

TParser::StructDeclaratorContext* TParser::StructDeclaratorListContext::structDeclarator(size_t i) {
  return getRuleContext<TParser::StructDeclaratorContext>(i);
}

std::vector<tree::TerminalNode *> TParser::StructDeclaratorListContext::Comma() {
  return getTokens(TParser::Comma);
}

tree::TerminalNode* TParser::StructDeclaratorListContext::Comma(size_t i) {
  return getToken(TParser::Comma, i);
}


size_t TParser::StructDeclaratorListContext::getRuleIndex() const {
  return TParser::RuleStructDeclaratorList;
}

void TParser::StructDeclaratorListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructDeclaratorList(this);
}

void TParser::StructDeclaratorListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructDeclaratorList(this);
}


std::any TParser::StructDeclaratorListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitStructDeclaratorList(this);
  else
    return visitor->visitChildren(this);
}

TParser::StructDeclaratorListContext* TParser::structDeclaratorList() {
  StructDeclaratorListContext *_localctx = _tracker.createInstance<StructDeclaratorListContext>(_ctx, getState());
  enterRule(_localctx, 26, TParser::RuleStructDeclaratorList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(190);
    structDeclarator();
    setState(195);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::Comma) {
      setState(191);
      match(TParser::Comma);
      setState(192);
      structDeclarator();
      setState(197);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StructDeclaratorContext ------------------------------------------------------------------

TParser::StructDeclaratorContext::StructDeclaratorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::DeclaratorContext* TParser::StructDeclaratorContext::declarator() {
  return getRuleContext<TParser::DeclaratorContext>(0);
}


size_t TParser::StructDeclaratorContext::getRuleIndex() const {
  return TParser::RuleStructDeclarator;
}

void TParser::StructDeclaratorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStructDeclarator(this);
}

void TParser::StructDeclaratorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStructDeclarator(this);
}


std::any TParser::StructDeclaratorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitStructDeclarator(this);
  else
    return visitor->visitChildren(this);
}

TParser::StructDeclaratorContext* TParser::structDeclarator() {
  StructDeclaratorContext *_localctx = _tracker.createInstance<StructDeclaratorContext>(_ctx, getState());
  enterRule(_localctx, 28, TParser::RuleStructDeclarator);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(198);
    declarator();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EnumSpecifierContext ------------------------------------------------------------------

TParser::EnumSpecifierContext::EnumSpecifierContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::EnumSpecifierContext::Enum() {
  return getToken(TParser::Enum, 0);
}

tree::TerminalNode* TParser::EnumSpecifierContext::LeftBrace() {
  return getToken(TParser::LeftBrace, 0);
}

TParser::EnumeratorListContext* TParser::EnumSpecifierContext::enumeratorList() {
  return getRuleContext<TParser::EnumeratorListContext>(0);
}

tree::TerminalNode* TParser::EnumSpecifierContext::RightBrace() {
  return getToken(TParser::RightBrace, 0);
}

tree::TerminalNode* TParser::EnumSpecifierContext::Identifier() {
  return getToken(TParser::Identifier, 0);
}


size_t TParser::EnumSpecifierContext::getRuleIndex() const {
  return TParser::RuleEnumSpecifier;
}

void TParser::EnumSpecifierContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEnumSpecifier(this);
}

void TParser::EnumSpecifierContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEnumSpecifier(this);
}


std::any TParser::EnumSpecifierContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitEnumSpecifier(this);
  else
    return visitor->visitChildren(this);
}

TParser::EnumSpecifierContext* TParser::enumSpecifier() {
  EnumSpecifierContext *_localctx = _tracker.createInstance<EnumSpecifierContext>(_ctx, getState());
  enterRule(_localctx, 30, TParser::RuleEnumSpecifier);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(200);
    match(TParser::Enum);
    setState(202);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TParser::Identifier) {
      setState(201);
      match(TParser::Identifier);
    }
    setState(204);
    match(TParser::LeftBrace);
    setState(205);
    enumeratorList();
    setState(206);
    match(TParser::RightBrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EnumeratorListContext ------------------------------------------------------------------

TParser::EnumeratorListContext::EnumeratorListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::EnumeratorContext *> TParser::EnumeratorListContext::enumerator() {
  return getRuleContexts<TParser::EnumeratorContext>();
}

TParser::EnumeratorContext* TParser::EnumeratorListContext::enumerator(size_t i) {
  return getRuleContext<TParser::EnumeratorContext>(i);
}

std::vector<tree::TerminalNode *> TParser::EnumeratorListContext::Comma() {
  return getTokens(TParser::Comma);
}

tree::TerminalNode* TParser::EnumeratorListContext::Comma(size_t i) {
  return getToken(TParser::Comma, i);
}


size_t TParser::EnumeratorListContext::getRuleIndex() const {
  return TParser::RuleEnumeratorList;
}

void TParser::EnumeratorListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEnumeratorList(this);
}

void TParser::EnumeratorListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEnumeratorList(this);
}


std::any TParser::EnumeratorListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitEnumeratorList(this);
  else
    return visitor->visitChildren(this);
}

TParser::EnumeratorListContext* TParser::enumeratorList() {
  EnumeratorListContext *_localctx = _tracker.createInstance<EnumeratorListContext>(_ctx, getState());
  enterRule(_localctx, 32, TParser::RuleEnumeratorList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(208);
    enumerator();
    setState(213);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(209);
        match(TParser::Comma);
        setState(210);
        enumerator(); 
      }
      setState(215);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    }
    setState(217);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TParser::Comma) {
      setState(216);
      match(TParser::Comma);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EnumeratorContext ------------------------------------------------------------------

TParser::EnumeratorContext::EnumeratorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::EnumeratorContext::Identifier() {
  return getToken(TParser::Identifier, 0);
}

tree::TerminalNode* TParser::EnumeratorContext::Assign() {
  return getToken(TParser::Assign, 0);
}

TParser::ConstantExpressionContext* TParser::EnumeratorContext::constantExpression() {
  return getRuleContext<TParser::ConstantExpressionContext>(0);
}


size_t TParser::EnumeratorContext::getRuleIndex() const {
  return TParser::RuleEnumerator;
}

void TParser::EnumeratorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEnumerator(this);
}

void TParser::EnumeratorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEnumerator(this);
}


std::any TParser::EnumeratorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitEnumerator(this);
  else
    return visitor->visitChildren(this);
}

TParser::EnumeratorContext* TParser::enumerator() {
  EnumeratorContext *_localctx = _tracker.createInstance<EnumeratorContext>(_ctx, getState());
  enterRule(_localctx, 34, TParser::RuleEnumerator);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(219);
    match(TParser::Identifier);
    setState(222);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TParser::Assign) {
      setState(220);
      match(TParser::Assign);
      setState(221);
      constantExpression();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InitDeclaratorListContext ------------------------------------------------------------------

TParser::InitDeclaratorListContext::InitDeclaratorListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::InitDeclaratorContext *> TParser::InitDeclaratorListContext::initDeclarator() {
  return getRuleContexts<TParser::InitDeclaratorContext>();
}

TParser::InitDeclaratorContext* TParser::InitDeclaratorListContext::initDeclarator(size_t i) {
  return getRuleContext<TParser::InitDeclaratorContext>(i);
}

std::vector<tree::TerminalNode *> TParser::InitDeclaratorListContext::Comma() {
  return getTokens(TParser::Comma);
}

tree::TerminalNode* TParser::InitDeclaratorListContext::Comma(size_t i) {
  return getToken(TParser::Comma, i);
}


size_t TParser::InitDeclaratorListContext::getRuleIndex() const {
  return TParser::RuleInitDeclaratorList;
}

void TParser::InitDeclaratorListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInitDeclaratorList(this);
}

void TParser::InitDeclaratorListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInitDeclaratorList(this);
}


std::any TParser::InitDeclaratorListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitInitDeclaratorList(this);
  else
    return visitor->visitChildren(this);
}

TParser::InitDeclaratorListContext* TParser::initDeclaratorList() {
  InitDeclaratorListContext *_localctx = _tracker.createInstance<InitDeclaratorListContext>(_ctx, getState());
  enterRule(_localctx, 36, TParser::RuleInitDeclaratorList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(224);
    initDeclarator();
    setState(229);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::Comma) {
      setState(225);
      match(TParser::Comma);
      setState(226);
      initDeclarator();
      setState(231);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InitDeclaratorContext ------------------------------------------------------------------

TParser::InitDeclaratorContext::InitDeclaratorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::DeclaratorContext* TParser::InitDeclaratorContext::declarator() {
  return getRuleContext<TParser::DeclaratorContext>(0);
}

tree::TerminalNode* TParser::InitDeclaratorContext::Assign() {
  return getToken(TParser::Assign, 0);
}

TParser::InitializerContext* TParser::InitDeclaratorContext::initializer() {
  return getRuleContext<TParser::InitializerContext>(0);
}


size_t TParser::InitDeclaratorContext::getRuleIndex() const {
  return TParser::RuleInitDeclarator;
}

void TParser::InitDeclaratorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInitDeclarator(this);
}

void TParser::InitDeclaratorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInitDeclarator(this);
}


std::any TParser::InitDeclaratorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitInitDeclarator(this);
  else
    return visitor->visitChildren(this);
}

TParser::InitDeclaratorContext* TParser::initDeclarator() {
  InitDeclaratorContext *_localctx = _tracker.createInstance<InitDeclaratorContext>(_ctx, getState());
  enterRule(_localctx, 38, TParser::RuleInitDeclarator);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(232);
    declarator();
    setState(235);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TParser::Assign) {
      setState(233);
      match(TParser::Assign);
      setState(234);
      initializer();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclaratorContext ------------------------------------------------------------------

TParser::DeclaratorContext::DeclaratorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::DirectDeclaratorContext* TParser::DeclaratorContext::directDeclarator() {
  return getRuleContext<TParser::DirectDeclaratorContext>(0);
}

TParser::PointerContext* TParser::DeclaratorContext::pointer() {
  return getRuleContext<TParser::PointerContext>(0);
}


size_t TParser::DeclaratorContext::getRuleIndex() const {
  return TParser::RuleDeclarator;
}

void TParser::DeclaratorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclarator(this);
}

void TParser::DeclaratorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclarator(this);
}


std::any TParser::DeclaratorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitDeclarator(this);
  else
    return visitor->visitChildren(this);
}

TParser::DeclaratorContext* TParser::declarator() {
  DeclaratorContext *_localctx = _tracker.createInstance<DeclaratorContext>(_ctx, getState());
  enterRule(_localctx, 40, TParser::RuleDeclarator);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(238);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TParser::Star) {
      setState(237);
      pointer();
    }
    setState(240);
    directDeclarator(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DirectDeclaratorContext ------------------------------------------------------------------

TParser::DirectDeclaratorContext::DirectDeclaratorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::DirectDeclaratorContext::Identifier() {
  return getToken(TParser::Identifier, 0);
}

tree::TerminalNode* TParser::DirectDeclaratorContext::LeftParen() {
  return getToken(TParser::LeftParen, 0);
}

TParser::DeclaratorContext* TParser::DirectDeclaratorContext::declarator() {
  return getRuleContext<TParser::DeclaratorContext>(0);
}

tree::TerminalNode* TParser::DirectDeclaratorContext::RightParen() {
  return getToken(TParser::RightParen, 0);
}

TParser::DirectDeclaratorContext* TParser::DirectDeclaratorContext::directDeclarator() {
  return getRuleContext<TParser::DirectDeclaratorContext>(0);
}

TParser::ParameterTypeListContext* TParser::DirectDeclaratorContext::parameterTypeList() {
  return getRuleContext<TParser::ParameterTypeListContext>(0);
}


size_t TParser::DirectDeclaratorContext::getRuleIndex() const {
  return TParser::RuleDirectDeclarator;
}

void TParser::DirectDeclaratorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDirectDeclarator(this);
}

void TParser::DirectDeclaratorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDirectDeclarator(this);
}


std::any TParser::DirectDeclaratorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitDirectDeclarator(this);
  else
    return visitor->visitChildren(this);
}


TParser::DirectDeclaratorContext* TParser::directDeclarator() {
   return directDeclarator(0);
}

TParser::DirectDeclaratorContext* TParser::directDeclarator(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  TParser::DirectDeclaratorContext *_localctx = _tracker.createInstance<DirectDeclaratorContext>(_ctx, parentState);
  TParser::DirectDeclaratorContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 42;
  enterRecursionRule(_localctx, 42, TParser::RuleDirectDeclarator, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(248);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::Identifier: {
        setState(243);
        match(TParser::Identifier);
        break;
      }

      case TParser::LeftParen: {
        setState(244);
        match(TParser::LeftParen);
        setState(245);
        declarator();
        setState(246);
        match(TParser::RightParen);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(258);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<DirectDeclaratorContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleDirectDeclarator);
        setState(250);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(251);
        match(TParser::LeftParen);
        setState(253);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & 858380596324) != 0)) {
          setState(252);
          parameterTypeList();
        }
        setState(255);
        match(TParser::RightParen); 
      }
      setState(260);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 19, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- PointerContext ------------------------------------------------------------------

TParser::PointerContext::PointerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::PointerContext::Star() {
  return getToken(TParser::Star, 0);
}

std::vector<TParser::TypeQualifierContext *> TParser::PointerContext::typeQualifier() {
  return getRuleContexts<TParser::TypeQualifierContext>();
}

TParser::TypeQualifierContext* TParser::PointerContext::typeQualifier(size_t i) {
  return getRuleContext<TParser::TypeQualifierContext>(i);
}

TParser::PointerContext* TParser::PointerContext::pointer() {
  return getRuleContext<TParser::PointerContext>(0);
}


size_t TParser::PointerContext::getRuleIndex() const {
  return TParser::RulePointer;
}

void TParser::PointerContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPointer(this);
}

void TParser::PointerContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPointer(this);
}


std::any TParser::PointerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitPointer(this);
  else
    return visitor->visitChildren(this);
}

TParser::PointerContext* TParser::pointer() {
  PointerContext *_localctx = _tracker.createInstance<PointerContext>(_ctx, getState());
  enterRule(_localctx, 44, TParser::RulePointer);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(261);
    match(TParser::Star);
    setState(265);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 292061970496) != 0)) {
      setState(262);
      typeQualifier();
      setState(267);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(269);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TParser::Star) {
      setState(268);
      pointer();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterTypeListContext ------------------------------------------------------------------

TParser::ParameterTypeListContext::ParameterTypeListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::ParameterListContext* TParser::ParameterTypeListContext::parameterList() {
  return getRuleContext<TParser::ParameterListContext>(0);
}


size_t TParser::ParameterTypeListContext::getRuleIndex() const {
  return TParser::RuleParameterTypeList;
}

void TParser::ParameterTypeListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParameterTypeList(this);
}

void TParser::ParameterTypeListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParameterTypeList(this);
}


std::any TParser::ParameterTypeListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitParameterTypeList(this);
  else
    return visitor->visitChildren(this);
}

TParser::ParameterTypeListContext* TParser::parameterTypeList() {
  ParameterTypeListContext *_localctx = _tracker.createInstance<ParameterTypeListContext>(_ctx, getState());
  enterRule(_localctx, 46, TParser::RuleParameterTypeList);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(271);
    parameterList();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterListContext ------------------------------------------------------------------

TParser::ParameterListContext::ParameterListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::ParameterDeclarationContext *> TParser::ParameterListContext::parameterDeclaration() {
  return getRuleContexts<TParser::ParameterDeclarationContext>();
}

TParser::ParameterDeclarationContext* TParser::ParameterListContext::parameterDeclaration(size_t i) {
  return getRuleContext<TParser::ParameterDeclarationContext>(i);
}

std::vector<tree::TerminalNode *> TParser::ParameterListContext::Comma() {
  return getTokens(TParser::Comma);
}

tree::TerminalNode* TParser::ParameterListContext::Comma(size_t i) {
  return getToken(TParser::Comma, i);
}


size_t TParser::ParameterListContext::getRuleIndex() const {
  return TParser::RuleParameterList;
}

void TParser::ParameterListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParameterList(this);
}

void TParser::ParameterListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParameterList(this);
}


std::any TParser::ParameterListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitParameterList(this);
  else
    return visitor->visitChildren(this);
}

TParser::ParameterListContext* TParser::parameterList() {
  ParameterListContext *_localctx = _tracker.createInstance<ParameterListContext>(_ctx, getState());
  enterRule(_localctx, 48, TParser::RuleParameterList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(273);
    parameterDeclaration();
    setState(278);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::Comma) {
      setState(274);
      match(TParser::Comma);
      setState(275);
      parameterDeclaration();
      setState(280);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParameterDeclarationContext ------------------------------------------------------------------

TParser::ParameterDeclarationContext::ParameterDeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::DeclarationSpecifiersContext* TParser::ParameterDeclarationContext::declarationSpecifiers() {
  return getRuleContext<TParser::DeclarationSpecifiersContext>(0);
}

TParser::DeclaratorContext* TParser::ParameterDeclarationContext::declarator() {
  return getRuleContext<TParser::DeclaratorContext>(0);
}


size_t TParser::ParameterDeclarationContext::getRuleIndex() const {
  return TParser::RuleParameterDeclaration;
}

void TParser::ParameterDeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParameterDeclaration(this);
}

void TParser::ParameterDeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParameterDeclaration(this);
}


std::any TParser::ParameterDeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitParameterDeclaration(this);
  else
    return visitor->visitChildren(this);
}

TParser::ParameterDeclarationContext* TParser::parameterDeclaration() {
  ParameterDeclarationContext *_localctx = _tracker.createInstance<ParameterDeclarationContext>(_ctx, getState());
  enterRule(_localctx, 50, TParser::RuleParameterDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(281);
    declarationSpecifiers();
    setState(282);
    declarator();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InitializerContext ------------------------------------------------------------------

TParser::InitializerContext::InitializerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::AssignmentExpressionContext* TParser::InitializerContext::assignmentExpression() {
  return getRuleContext<TParser::AssignmentExpressionContext>(0);
}

tree::TerminalNode* TParser::InitializerContext::LeftBrace() {
  return getToken(TParser::LeftBrace, 0);
}

TParser::InitializerListContext* TParser::InitializerContext::initializerList() {
  return getRuleContext<TParser::InitializerListContext>(0);
}

tree::TerminalNode* TParser::InitializerContext::RightBrace() {
  return getToken(TParser::RightBrace, 0);
}


size_t TParser::InitializerContext::getRuleIndex() const {
  return TParser::RuleInitializer;
}

void TParser::InitializerContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInitializer(this);
}

void TParser::InitializerContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInitializer(this);
}


std::any TParser::InitializerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitInitializer(this);
  else
    return visitor->visitChildren(this);
}

TParser::InitializerContext* TParser::initializer() {
  InitializerContext *_localctx = _tracker.createInstance<InitializerContext>(_ctx, getState());
  enterRule(_localctx, 52, TParser::RuleInitializer);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(289);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::Identifier:
      case TParser::IntegerConstant:
      case TParser::FloatingConstant:
      case TParser::CharacterConstant:
      case TParser::StringLiteral:
      case TParser::LeftParen:
      case TParser::PlusPlus:
      case TParser::MinusMinus:
      case TParser::Plus:
      case TParser::Minus:
      case TParser::Star:
      case TParser::And:
      case TParser::Tilde:
      case TParser::Not: {
        enterOuterAlt(_localctx, 1);
        setState(284);
        assignmentExpression();
        break;
      }

      case TParser::LeftBrace: {
        enterOuterAlt(_localctx, 2);
        setState(285);
        match(TParser::LeftBrace);
        setState(286);
        initializerList();
        setState(287);
        match(TParser::RightBrace);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InitializerListContext ------------------------------------------------------------------

TParser::InitializerListContext::InitializerListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::InitializerContext *> TParser::InitializerListContext::initializer() {
  return getRuleContexts<TParser::InitializerContext>();
}

TParser::InitializerContext* TParser::InitializerListContext::initializer(size_t i) {
  return getRuleContext<TParser::InitializerContext>(i);
}

std::vector<tree::TerminalNode *> TParser::InitializerListContext::Comma() {
  return getTokens(TParser::Comma);
}

tree::TerminalNode* TParser::InitializerListContext::Comma(size_t i) {
  return getToken(TParser::Comma, i);
}


size_t TParser::InitializerListContext::getRuleIndex() const {
  return TParser::RuleInitializerList;
}

void TParser::InitializerListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInitializerList(this);
}

void TParser::InitializerListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInitializerList(this);
}


std::any TParser::InitializerListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitInitializerList(this);
  else
    return visitor->visitChildren(this);
}

TParser::InitializerListContext* TParser::initializerList() {
  InitializerListContext *_localctx = _tracker.createInstance<InitializerListContext>(_ctx, getState());
  enterRule(_localctx, 54, TParser::RuleInitializerList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(291);
    initializer();
    setState(296);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(292);
        match(TParser::Comma);
        setState(293);
        initializer(); 
      }
      setState(298);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx);
    }
    setState(300);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TParser::Comma) {
      setState(299);
      match(TParser::Comma);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

TParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::LabeledStatementContext* TParser::StatementContext::labeledStatement() {
  return getRuleContext<TParser::LabeledStatementContext>(0);
}

TParser::CompoundStatementContext* TParser::StatementContext::compoundStatement() {
  return getRuleContext<TParser::CompoundStatementContext>(0);
}

TParser::ExpressionStatementContext* TParser::StatementContext::expressionStatement() {
  return getRuleContext<TParser::ExpressionStatementContext>(0);
}

TParser::SelectionStatementContext* TParser::StatementContext::selectionStatement() {
  return getRuleContext<TParser::SelectionStatementContext>(0);
}

TParser::IterationStatementContext* TParser::StatementContext::iterationStatement() {
  return getRuleContext<TParser::IterationStatementContext>(0);
}

TParser::JumpStatementContext* TParser::StatementContext::jumpStatement() {
  return getRuleContext<TParser::JumpStatementContext>(0);
}


size_t TParser::StatementContext::getRuleIndex() const {
  return TParser::RuleStatement;
}

void TParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void TParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}


std::any TParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

TParser::StatementContext* TParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 56, TParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(308);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 26, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(302);
      labeledStatement();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(303);
      compoundStatement();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(304);
      expressionStatement();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(305);
      selectionStatement();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(306);
      iterationStatement();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(307);
      jumpStatement();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LabeledStatementContext ------------------------------------------------------------------

TParser::LabeledStatementContext::LabeledStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::LabeledStatementContext::Identifier() {
  return getToken(TParser::Identifier, 0);
}

tree::TerminalNode* TParser::LabeledStatementContext::Colon() {
  return getToken(TParser::Colon, 0);
}

TParser::StatementContext* TParser::LabeledStatementContext::statement() {
  return getRuleContext<TParser::StatementContext>(0);
}

tree::TerminalNode* TParser::LabeledStatementContext::Case() {
  return getToken(TParser::Case, 0);
}

TParser::ConstantExpressionContext* TParser::LabeledStatementContext::constantExpression() {
  return getRuleContext<TParser::ConstantExpressionContext>(0);
}

tree::TerminalNode* TParser::LabeledStatementContext::Default() {
  return getToken(TParser::Default, 0);
}


size_t TParser::LabeledStatementContext::getRuleIndex() const {
  return TParser::RuleLabeledStatement;
}

void TParser::LabeledStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLabeledStatement(this);
}

void TParser::LabeledStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLabeledStatement(this);
}


std::any TParser::LabeledStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitLabeledStatement(this);
  else
    return visitor->visitChildren(this);
}

TParser::LabeledStatementContext* TParser::labeledStatement() {
  LabeledStatementContext *_localctx = _tracker.createInstance<LabeledStatementContext>(_ctx, getState());
  enterRule(_localctx, 58, TParser::RuleLabeledStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(321);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::Identifier: {
        enterOuterAlt(_localctx, 1);
        setState(310);
        match(TParser::Identifier);
        setState(311);
        match(TParser::Colon);
        setState(312);
        statement();
        break;
      }

      case TParser::Case: {
        enterOuterAlt(_localctx, 2);
        setState(313);
        match(TParser::Case);
        setState(314);
        constantExpression();
        setState(315);
        match(TParser::Colon);
        setState(316);
        statement();
        break;
      }

      case TParser::Default: {
        enterOuterAlt(_localctx, 3);
        setState(318);
        match(TParser::Default);
        setState(319);
        match(TParser::Colon);
        setState(320);
        statement();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CompoundStatementContext ------------------------------------------------------------------

TParser::CompoundStatementContext::CompoundStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::CompoundStatementContext::LeftBrace() {
  return getToken(TParser::LeftBrace, 0);
}

tree::TerminalNode* TParser::CompoundStatementContext::RightBrace() {
  return getToken(TParser::RightBrace, 0);
}

TParser::BlockItemListContext* TParser::CompoundStatementContext::blockItemList() {
  return getRuleContext<TParser::BlockItemListContext>(0);
}


size_t TParser::CompoundStatementContext::getRuleIndex() const {
  return TParser::RuleCompoundStatement;
}

void TParser::CompoundStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCompoundStatement(this);
}

void TParser::CompoundStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCompoundStatement(this);
}


std::any TParser::CompoundStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitCompoundStatement(this);
  else
    return visitor->visitChildren(this);
}

TParser::CompoundStatementContext* TParser::compoundStatement() {
  CompoundStatementContext *_localctx = _tracker.createInstance<CompoundStatementContext>(_ctx, getState());
  enterRule(_localctx, 60, TParser::RuleCompoundStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(323);
    match(TParser::LeftBrace);
    setState(325);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -2143994004319242244) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 1048979) != 0)) {
      setState(324);
      blockItemList();
    }
    setState(327);
    match(TParser::RightBrace);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemListContext ------------------------------------------------------------------

TParser::BlockItemListContext::BlockItemListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::BlockItemContext *> TParser::BlockItemListContext::blockItem() {
  return getRuleContexts<TParser::BlockItemContext>();
}

TParser::BlockItemContext* TParser::BlockItemListContext::blockItem(size_t i) {
  return getRuleContext<TParser::BlockItemContext>(i);
}


size_t TParser::BlockItemListContext::getRuleIndex() const {
  return TParser::RuleBlockItemList;
}

void TParser::BlockItemListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlockItemList(this);
}

void TParser::BlockItemListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlockItemList(this);
}


std::any TParser::BlockItemListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitBlockItemList(this);
  else
    return visitor->visitChildren(this);
}

TParser::BlockItemListContext* TParser::blockItemList() {
  BlockItemListContext *_localctx = _tracker.createInstance<BlockItemListContext>(_ctx, getState());
  enterRule(_localctx, 62, TParser::RuleBlockItemList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(330); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(329);
      blockItem();
      setState(332); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & -2143994004319242244) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & 1048979) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockItemContext ------------------------------------------------------------------

TParser::BlockItemContext::BlockItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::DeclarationContext* TParser::BlockItemContext::declaration() {
  return getRuleContext<TParser::DeclarationContext>(0);
}

TParser::StatementContext* TParser::BlockItemContext::statement() {
  return getRuleContext<TParser::StatementContext>(0);
}


size_t TParser::BlockItemContext::getRuleIndex() const {
  return TParser::RuleBlockItem;
}

void TParser::BlockItemContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlockItem(this);
}

void TParser::BlockItemContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlockItem(this);
}


std::any TParser::BlockItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitBlockItem(this);
  else
    return visitor->visitChildren(this);
}

TParser::BlockItemContext* TParser::blockItem() {
  BlockItemContext *_localctx = _tracker.createInstance<BlockItemContext>(_ctx, getState());
  enterRule(_localctx, 64, TParser::RuleBlockItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(336);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::Auto:
      case TParser::Char:
      case TParser::Const:
      case TParser::Double:
      case TParser::Enum:
      case TParser::Extern:
      case TParser::Float:
      case TParser::Int:
      case TParser::Long:
      case TParser::Register:
      case TParser::Restrict:
      case TParser::Short:
      case TParser::Signed:
      case TParser::Static:
      case TParser::Struct:
      case TParser::Typedef:
      case TParser::Union:
      case TParser::Unsigned:
      case TParser::Void:
      case TParser::Volatile:
      case TParser::Atomic:
      case TParser::Bool: {
        enterOuterAlt(_localctx, 1);
        setState(334);
        declaration();
        break;
      }

      case TParser::Break:
      case TParser::Case:
      case TParser::Continue:
      case TParser::Default:
      case TParser::Do:
      case TParser::For:
      case TParser::Goto:
      case TParser::If:
      case TParser::Return:
      case TParser::Switch:
      case TParser::While:
      case TParser::Identifier:
      case TParser::IntegerConstant:
      case TParser::FloatingConstant:
      case TParser::CharacterConstant:
      case TParser::StringLiteral:
      case TParser::LeftParen:
      case TParser::LeftBrace:
      case TParser::PlusPlus:
      case TParser::MinusMinus:
      case TParser::Plus:
      case TParser::Minus:
      case TParser::Star:
      case TParser::And:
      case TParser::Tilde:
      case TParser::Not:
      case TParser::Semi: {
        enterOuterAlt(_localctx, 2);
        setState(335);
        statement();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionStatementContext ------------------------------------------------------------------

TParser::ExpressionStatementContext::ExpressionStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::ExpressionStatementContext::Semi() {
  return getToken(TParser::Semi, 0);
}

TParser::ExpressionContext* TParser::ExpressionStatementContext::expression() {
  return getRuleContext<TParser::ExpressionContext>(0);
}


size_t TParser::ExpressionStatementContext::getRuleIndex() const {
  return TParser::RuleExpressionStatement;
}

void TParser::ExpressionStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpressionStatement(this);
}

void TParser::ExpressionStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpressionStatement(this);
}


std::any TParser::ExpressionStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitExpressionStatement(this);
  else
    return visitor->visitChildren(this);
}

TParser::ExpressionStatementContext* TParser::expressionStatement() {
  ExpressionStatementContext *_localctx = _tracker.createInstance<ExpressionStatementContext>(_ctx, getState());
  enterRule(_localctx, 66, TParser::RuleExpressionStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(339);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (((((_la - 48) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 48)) & 26468415) != 0)) {
      setState(338);
      expression();
    }
    setState(341);
    match(TParser::Semi);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SelectionStatementContext ------------------------------------------------------------------

TParser::SelectionStatementContext::SelectionStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::SelectionStatementContext::If() {
  return getToken(TParser::If, 0);
}

tree::TerminalNode* TParser::SelectionStatementContext::LeftParen() {
  return getToken(TParser::LeftParen, 0);
}

TParser::ExpressionContext* TParser::SelectionStatementContext::expression() {
  return getRuleContext<TParser::ExpressionContext>(0);
}

tree::TerminalNode* TParser::SelectionStatementContext::RightParen() {
  return getToken(TParser::RightParen, 0);
}

std::vector<TParser::StatementContext *> TParser::SelectionStatementContext::statement() {
  return getRuleContexts<TParser::StatementContext>();
}

TParser::StatementContext* TParser::SelectionStatementContext::statement(size_t i) {
  return getRuleContext<TParser::StatementContext>(i);
}

tree::TerminalNode* TParser::SelectionStatementContext::Else() {
  return getToken(TParser::Else, 0);
}

tree::TerminalNode* TParser::SelectionStatementContext::Switch() {
  return getToken(TParser::Switch, 0);
}


size_t TParser::SelectionStatementContext::getRuleIndex() const {
  return TParser::RuleSelectionStatement;
}

void TParser::SelectionStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSelectionStatement(this);
}

void TParser::SelectionStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSelectionStatement(this);
}


std::any TParser::SelectionStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitSelectionStatement(this);
  else
    return visitor->visitChildren(this);
}

TParser::SelectionStatementContext* TParser::selectionStatement() {
  SelectionStatementContext *_localctx = _tracker.createInstance<SelectionStatementContext>(_ctx, getState());
  enterRule(_localctx, 68, TParser::RuleSelectionStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(358);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::If: {
        enterOuterAlt(_localctx, 1);
        setState(343);
        match(TParser::If);
        setState(344);
        match(TParser::LeftParen);
        setState(345);
        expression();
        setState(346);
        match(TParser::RightParen);
        setState(347);
        statement();
        setState(350);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 32, _ctx)) {
        case 1: {
          setState(348);
          match(TParser::Else);
          setState(349);
          statement();
          break;
        }

        default:
          break;
        }
        break;
      }

      case TParser::Switch: {
        enterOuterAlt(_localctx, 2);
        setState(352);
        match(TParser::Switch);
        setState(353);
        match(TParser::LeftParen);
        setState(354);
        expression();
        setState(355);
        match(TParser::RightParen);
        setState(356);
        statement();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IterationStatementContext ------------------------------------------------------------------

TParser::IterationStatementContext::IterationStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::IterationStatementContext::While() {
  return getToken(TParser::While, 0);
}

tree::TerminalNode* TParser::IterationStatementContext::LeftParen() {
  return getToken(TParser::LeftParen, 0);
}

TParser::ExpressionContext* TParser::IterationStatementContext::expression() {
  return getRuleContext<TParser::ExpressionContext>(0);
}

tree::TerminalNode* TParser::IterationStatementContext::RightParen() {
  return getToken(TParser::RightParen, 0);
}

TParser::StatementContext* TParser::IterationStatementContext::statement() {
  return getRuleContext<TParser::StatementContext>(0);
}

tree::TerminalNode* TParser::IterationStatementContext::Do() {
  return getToken(TParser::Do, 0);
}

tree::TerminalNode* TParser::IterationStatementContext::Semi() {
  return getToken(TParser::Semi, 0);
}

tree::TerminalNode* TParser::IterationStatementContext::For() {
  return getToken(TParser::For, 0);
}

std::vector<TParser::ExpressionStatementContext *> TParser::IterationStatementContext::expressionStatement() {
  return getRuleContexts<TParser::ExpressionStatementContext>();
}

TParser::ExpressionStatementContext* TParser::IterationStatementContext::expressionStatement(size_t i) {
  return getRuleContext<TParser::ExpressionStatementContext>(i);
}


size_t TParser::IterationStatementContext::getRuleIndex() const {
  return TParser::RuleIterationStatement;
}

void TParser::IterationStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIterationStatement(this);
}

void TParser::IterationStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIterationStatement(this);
}


std::any TParser::IterationStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitIterationStatement(this);
  else
    return visitor->visitChildren(this);
}

TParser::IterationStatementContext* TParser::iterationStatement() {
  IterationStatementContext *_localctx = _tracker.createInstance<IterationStatementContext>(_ctx, getState());
  enterRule(_localctx, 70, TParser::RuleIterationStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(384);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::While: {
        enterOuterAlt(_localctx, 1);
        setState(360);
        match(TParser::While);
        setState(361);
        match(TParser::LeftParen);
        setState(362);
        expression();
        setState(363);
        match(TParser::RightParen);
        setState(364);
        statement();
        break;
      }

      case TParser::Do: {
        enterOuterAlt(_localctx, 2);
        setState(366);
        match(TParser::Do);
        setState(367);
        statement();
        setState(368);
        match(TParser::While);
        setState(369);
        match(TParser::LeftParen);
        setState(370);
        expression();
        setState(371);
        match(TParser::RightParen);
        setState(372);
        match(TParser::Semi);
        break;
      }

      case TParser::For: {
        enterOuterAlt(_localctx, 3);
        setState(374);
        match(TParser::For);
        setState(375);
        match(TParser::LeftParen);
        setState(376);
        expressionStatement();
        setState(377);
        expressionStatement();
        setState(379);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 48) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 48)) & 26468415) != 0)) {
          setState(378);
          expression();
        }
        setState(381);
        match(TParser::RightParen);
        setState(382);
        statement();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- JumpStatementContext ------------------------------------------------------------------

TParser::JumpStatementContext::JumpStatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::JumpStatementContext::Goto() {
  return getToken(TParser::Goto, 0);
}

tree::TerminalNode* TParser::JumpStatementContext::Identifier() {
  return getToken(TParser::Identifier, 0);
}

tree::TerminalNode* TParser::JumpStatementContext::Semi() {
  return getToken(TParser::Semi, 0);
}

tree::TerminalNode* TParser::JumpStatementContext::Continue() {
  return getToken(TParser::Continue, 0);
}

tree::TerminalNode* TParser::JumpStatementContext::Break() {
  return getToken(TParser::Break, 0);
}

tree::TerminalNode* TParser::JumpStatementContext::Return() {
  return getToken(TParser::Return, 0);
}

TParser::ExpressionContext* TParser::JumpStatementContext::expression() {
  return getRuleContext<TParser::ExpressionContext>(0);
}


size_t TParser::JumpStatementContext::getRuleIndex() const {
  return TParser::RuleJumpStatement;
}

void TParser::JumpStatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterJumpStatement(this);
}

void TParser::JumpStatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitJumpStatement(this);
}


std::any TParser::JumpStatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitJumpStatement(this);
  else
    return visitor->visitChildren(this);
}

TParser::JumpStatementContext* TParser::jumpStatement() {
  JumpStatementContext *_localctx = _tracker.createInstance<JumpStatementContext>(_ctx, getState());
  enterRule(_localctx, 72, TParser::RuleJumpStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(398);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::Goto: {
        enterOuterAlt(_localctx, 1);
        setState(386);
        match(TParser::Goto);
        setState(387);
        match(TParser::Identifier);
        setState(388);
        match(TParser::Semi);
        break;
      }

      case TParser::Continue: {
        enterOuterAlt(_localctx, 2);
        setState(389);
        match(TParser::Continue);
        setState(390);
        match(TParser::Semi);
        break;
      }

      case TParser::Break: {
        enterOuterAlt(_localctx, 3);
        setState(391);
        match(TParser::Break);
        setState(392);
        match(TParser::Semi);
        break;
      }

      case TParser::Return: {
        enterOuterAlt(_localctx, 4);
        setState(393);
        match(TParser::Return);
        setState(395);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (((((_la - 48) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 48)) & 26468415) != 0)) {
          setState(394);
          expression();
        }
        setState(397);
        match(TParser::Semi);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

TParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::AssignmentExpressionContext *> TParser::ExpressionContext::assignmentExpression() {
  return getRuleContexts<TParser::AssignmentExpressionContext>();
}

TParser::AssignmentExpressionContext* TParser::ExpressionContext::assignmentExpression(size_t i) {
  return getRuleContext<TParser::AssignmentExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::ExpressionContext::Comma() {
  return getTokens(TParser::Comma);
}

tree::TerminalNode* TParser::ExpressionContext::Comma(size_t i) {
  return getToken(TParser::Comma, i);
}


size_t TParser::ExpressionContext::getRuleIndex() const {
  return TParser::RuleExpression;
}

void TParser::ExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpression(this);
}

void TParser::ExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpression(this);
}


std::any TParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::ExpressionContext* TParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 74, TParser::RuleExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(400);
    assignmentExpression();
    setState(405);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::Comma) {
      setState(401);
      match(TParser::Comma);
      setState(402);
      assignmentExpression();
      setState(407);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentExpressionContext ------------------------------------------------------------------

TParser::AssignmentExpressionContext::AssignmentExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::ConditionalExpressionContext* TParser::AssignmentExpressionContext::conditionalExpression() {
  return getRuleContext<TParser::ConditionalExpressionContext>(0);
}

TParser::UnaryExpressionContext* TParser::AssignmentExpressionContext::unaryExpression() {
  return getRuleContext<TParser::UnaryExpressionContext>(0);
}

tree::TerminalNode* TParser::AssignmentExpressionContext::Assign() {
  return getToken(TParser::Assign, 0);
}

TParser::AssignmentExpressionContext* TParser::AssignmentExpressionContext::assignmentExpression() {
  return getRuleContext<TParser::AssignmentExpressionContext>(0);
}


size_t TParser::AssignmentExpressionContext::getRuleIndex() const {
  return TParser::RuleAssignmentExpression;
}

void TParser::AssignmentExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignmentExpression(this);
}

void TParser::AssignmentExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignmentExpression(this);
}


std::any TParser::AssignmentExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitAssignmentExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::AssignmentExpressionContext* TParser::assignmentExpression() {
  AssignmentExpressionContext *_localctx = _tracker.createInstance<AssignmentExpressionContext>(_ctx, getState());
  enterRule(_localctx, 76, TParser::RuleAssignmentExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(413);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 39, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(408);
      conditionalExpression();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(409);
      unaryExpression();
      setState(410);
      match(TParser::Assign);
      setState(411);
      assignmentExpression();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConditionalExpressionContext ------------------------------------------------------------------

TParser::ConditionalExpressionContext::ConditionalExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::LogicalOrExpressionContext* TParser::ConditionalExpressionContext::logicalOrExpression() {
  return getRuleContext<TParser::LogicalOrExpressionContext>(0);
}

tree::TerminalNode* TParser::ConditionalExpressionContext::Question() {
  return getToken(TParser::Question, 0);
}

TParser::ExpressionContext* TParser::ConditionalExpressionContext::expression() {
  return getRuleContext<TParser::ExpressionContext>(0);
}

tree::TerminalNode* TParser::ConditionalExpressionContext::Colon() {
  return getToken(TParser::Colon, 0);
}

TParser::ConditionalExpressionContext* TParser::ConditionalExpressionContext::conditionalExpression() {
  return getRuleContext<TParser::ConditionalExpressionContext>(0);
}


size_t TParser::ConditionalExpressionContext::getRuleIndex() const {
  return TParser::RuleConditionalExpression;
}

void TParser::ConditionalExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConditionalExpression(this);
}

void TParser::ConditionalExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConditionalExpression(this);
}


std::any TParser::ConditionalExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitConditionalExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::ConditionalExpressionContext* TParser::conditionalExpression() {
  ConditionalExpressionContext *_localctx = _tracker.createInstance<ConditionalExpressionContext>(_ctx, getState());
  enterRule(_localctx, 78, TParser::RuleConditionalExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(415);
    logicalOrExpression();
    setState(421);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TParser::Question) {
      setState(416);
      match(TParser::Question);
      setState(417);
      expression();
      setState(418);
      match(TParser::Colon);
      setState(419);
      conditionalExpression();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstantExpressionContext ------------------------------------------------------------------

TParser::ConstantExpressionContext::ConstantExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::ConditionalExpressionContext* TParser::ConstantExpressionContext::conditionalExpression() {
  return getRuleContext<TParser::ConditionalExpressionContext>(0);
}


size_t TParser::ConstantExpressionContext::getRuleIndex() const {
  return TParser::RuleConstantExpression;
}

void TParser::ConstantExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstantExpression(this);
}

void TParser::ConstantExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstantExpression(this);
}


std::any TParser::ConstantExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitConstantExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::ConstantExpressionContext* TParser::constantExpression() {
  ConstantExpressionContext *_localctx = _tracker.createInstance<ConstantExpressionContext>(_ctx, getState());
  enterRule(_localctx, 80, TParser::RuleConstantExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(423);
    conditionalExpression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalOrExpressionContext ------------------------------------------------------------------

TParser::LogicalOrExpressionContext::LogicalOrExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::LogicalAndExpressionContext *> TParser::LogicalOrExpressionContext::logicalAndExpression() {
  return getRuleContexts<TParser::LogicalAndExpressionContext>();
}

TParser::LogicalAndExpressionContext* TParser::LogicalOrExpressionContext::logicalAndExpression(size_t i) {
  return getRuleContext<TParser::LogicalAndExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::LogicalOrExpressionContext::OrOr() {
  return getTokens(TParser::OrOr);
}

tree::TerminalNode* TParser::LogicalOrExpressionContext::OrOr(size_t i) {
  return getToken(TParser::OrOr, i);
}


size_t TParser::LogicalOrExpressionContext::getRuleIndex() const {
  return TParser::RuleLogicalOrExpression;
}

void TParser::LogicalOrExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLogicalOrExpression(this);
}

void TParser::LogicalOrExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLogicalOrExpression(this);
}


std::any TParser::LogicalOrExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitLogicalOrExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::LogicalOrExpressionContext* TParser::logicalOrExpression() {
  LogicalOrExpressionContext *_localctx = _tracker.createInstance<LogicalOrExpressionContext>(_ctx, getState());
  enterRule(_localctx, 82, TParser::RuleLogicalOrExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(425);
    logicalAndExpression();
    setState(430);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::OrOr) {
      setState(426);
      match(TParser::OrOr);
      setState(427);
      logicalAndExpression();
      setState(432);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalAndExpressionContext ------------------------------------------------------------------

TParser::LogicalAndExpressionContext::LogicalAndExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::InclusiveOrExpressionContext *> TParser::LogicalAndExpressionContext::inclusiveOrExpression() {
  return getRuleContexts<TParser::InclusiveOrExpressionContext>();
}

TParser::InclusiveOrExpressionContext* TParser::LogicalAndExpressionContext::inclusiveOrExpression(size_t i) {
  return getRuleContext<TParser::InclusiveOrExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::LogicalAndExpressionContext::AndAnd() {
  return getTokens(TParser::AndAnd);
}

tree::TerminalNode* TParser::LogicalAndExpressionContext::AndAnd(size_t i) {
  return getToken(TParser::AndAnd, i);
}


size_t TParser::LogicalAndExpressionContext::getRuleIndex() const {
  return TParser::RuleLogicalAndExpression;
}

void TParser::LogicalAndExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLogicalAndExpression(this);
}

void TParser::LogicalAndExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLogicalAndExpression(this);
}


std::any TParser::LogicalAndExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitLogicalAndExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::LogicalAndExpressionContext* TParser::logicalAndExpression() {
  LogicalAndExpressionContext *_localctx = _tracker.createInstance<LogicalAndExpressionContext>(_ctx, getState());
  enterRule(_localctx, 84, TParser::RuleLogicalAndExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(433);
    inclusiveOrExpression();
    setState(438);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::AndAnd) {
      setState(434);
      match(TParser::AndAnd);
      setState(435);
      inclusiveOrExpression();
      setState(440);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InclusiveOrExpressionContext ------------------------------------------------------------------

TParser::InclusiveOrExpressionContext::InclusiveOrExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::ExclusiveOrExpressionContext *> TParser::InclusiveOrExpressionContext::exclusiveOrExpression() {
  return getRuleContexts<TParser::ExclusiveOrExpressionContext>();
}

TParser::ExclusiveOrExpressionContext* TParser::InclusiveOrExpressionContext::exclusiveOrExpression(size_t i) {
  return getRuleContext<TParser::ExclusiveOrExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::InclusiveOrExpressionContext::Or() {
  return getTokens(TParser::Or);
}

tree::TerminalNode* TParser::InclusiveOrExpressionContext::Or(size_t i) {
  return getToken(TParser::Or, i);
}


size_t TParser::InclusiveOrExpressionContext::getRuleIndex() const {
  return TParser::RuleInclusiveOrExpression;
}

void TParser::InclusiveOrExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInclusiveOrExpression(this);
}

void TParser::InclusiveOrExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInclusiveOrExpression(this);
}


std::any TParser::InclusiveOrExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitInclusiveOrExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::InclusiveOrExpressionContext* TParser::inclusiveOrExpression() {
  InclusiveOrExpressionContext *_localctx = _tracker.createInstance<InclusiveOrExpressionContext>(_ctx, getState());
  enterRule(_localctx, 86, TParser::RuleInclusiveOrExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(441);
    exclusiveOrExpression();
    setState(446);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::Or) {
      setState(442);
      match(TParser::Or);
      setState(443);
      exclusiveOrExpression();
      setState(448);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExclusiveOrExpressionContext ------------------------------------------------------------------

TParser::ExclusiveOrExpressionContext::ExclusiveOrExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::AndExpressionContext *> TParser::ExclusiveOrExpressionContext::andExpression() {
  return getRuleContexts<TParser::AndExpressionContext>();
}

TParser::AndExpressionContext* TParser::ExclusiveOrExpressionContext::andExpression(size_t i) {
  return getRuleContext<TParser::AndExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::ExclusiveOrExpressionContext::Caret() {
  return getTokens(TParser::Caret);
}

tree::TerminalNode* TParser::ExclusiveOrExpressionContext::Caret(size_t i) {
  return getToken(TParser::Caret, i);
}


size_t TParser::ExclusiveOrExpressionContext::getRuleIndex() const {
  return TParser::RuleExclusiveOrExpression;
}

void TParser::ExclusiveOrExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExclusiveOrExpression(this);
}

void TParser::ExclusiveOrExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExclusiveOrExpression(this);
}


std::any TParser::ExclusiveOrExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitExclusiveOrExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::ExclusiveOrExpressionContext* TParser::exclusiveOrExpression() {
  ExclusiveOrExpressionContext *_localctx = _tracker.createInstance<ExclusiveOrExpressionContext>(_ctx, getState());
  enterRule(_localctx, 88, TParser::RuleExclusiveOrExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(449);
    andExpression();
    setState(454);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::Caret) {
      setState(450);
      match(TParser::Caret);
      setState(451);
      andExpression();
      setState(456);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AndExpressionContext ------------------------------------------------------------------

TParser::AndExpressionContext::AndExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::EqualityExpressionContext *> TParser::AndExpressionContext::equalityExpression() {
  return getRuleContexts<TParser::EqualityExpressionContext>();
}

TParser::EqualityExpressionContext* TParser::AndExpressionContext::equalityExpression(size_t i) {
  return getRuleContext<TParser::EqualityExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::AndExpressionContext::And() {
  return getTokens(TParser::And);
}

tree::TerminalNode* TParser::AndExpressionContext::And(size_t i) {
  return getToken(TParser::And, i);
}


size_t TParser::AndExpressionContext::getRuleIndex() const {
  return TParser::RuleAndExpression;
}

void TParser::AndExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAndExpression(this);
}

void TParser::AndExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAndExpression(this);
}


std::any TParser::AndExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitAndExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::AndExpressionContext* TParser::andExpression() {
  AndExpressionContext *_localctx = _tracker.createInstance<AndExpressionContext>(_ctx, getState());
  enterRule(_localctx, 90, TParser::RuleAndExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(457);
    equalityExpression();
    setState(462);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::And) {
      setState(458);
      match(TParser::And);
      setState(459);
      equalityExpression();
      setState(464);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqualityExpressionContext ------------------------------------------------------------------

TParser::EqualityExpressionContext::EqualityExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::RelationalExpressionContext *> TParser::EqualityExpressionContext::relationalExpression() {
  return getRuleContexts<TParser::RelationalExpressionContext>();
}

TParser::RelationalExpressionContext* TParser::EqualityExpressionContext::relationalExpression(size_t i) {
  return getRuleContext<TParser::RelationalExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::EqualityExpressionContext::Equal() {
  return getTokens(TParser::Equal);
}

tree::TerminalNode* TParser::EqualityExpressionContext::Equal(size_t i) {
  return getToken(TParser::Equal, i);
}

std::vector<tree::TerminalNode *> TParser::EqualityExpressionContext::NotEqual() {
  return getTokens(TParser::NotEqual);
}

tree::TerminalNode* TParser::EqualityExpressionContext::NotEqual(size_t i) {
  return getToken(TParser::NotEqual, i);
}


size_t TParser::EqualityExpressionContext::getRuleIndex() const {
  return TParser::RuleEqualityExpression;
}

void TParser::EqualityExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEqualityExpression(this);
}

void TParser::EqualityExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEqualityExpression(this);
}


std::any TParser::EqualityExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitEqualityExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::EqualityExpressionContext* TParser::equalityExpression() {
  EqualityExpressionContext *_localctx = _tracker.createInstance<EqualityExpressionContext>(_ctx, getState());
  enterRule(_localctx, 92, TParser::RuleEqualityExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(465);
    relationalExpression();
    setState(470);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::Equal

    || _la == TParser::NotEqual) {
      setState(466);
      _la = _input->LA(1);
      if (!(_la == TParser::Equal

      || _la == TParser::NotEqual)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(467);
      relationalExpression();
      setState(472);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelationalExpressionContext ------------------------------------------------------------------

TParser::RelationalExpressionContext::RelationalExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::ShiftExpressionContext *> TParser::RelationalExpressionContext::shiftExpression() {
  return getRuleContexts<TParser::ShiftExpressionContext>();
}

TParser::ShiftExpressionContext* TParser::RelationalExpressionContext::shiftExpression(size_t i) {
  return getRuleContext<TParser::ShiftExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::RelationalExpressionContext::Less() {
  return getTokens(TParser::Less);
}

tree::TerminalNode* TParser::RelationalExpressionContext::Less(size_t i) {
  return getToken(TParser::Less, i);
}

std::vector<tree::TerminalNode *> TParser::RelationalExpressionContext::Greater() {
  return getTokens(TParser::Greater);
}

tree::TerminalNode* TParser::RelationalExpressionContext::Greater(size_t i) {
  return getToken(TParser::Greater, i);
}

std::vector<tree::TerminalNode *> TParser::RelationalExpressionContext::LessEqual() {
  return getTokens(TParser::LessEqual);
}

tree::TerminalNode* TParser::RelationalExpressionContext::LessEqual(size_t i) {
  return getToken(TParser::LessEqual, i);
}

std::vector<tree::TerminalNode *> TParser::RelationalExpressionContext::GreaterEqual() {
  return getTokens(TParser::GreaterEqual);
}

tree::TerminalNode* TParser::RelationalExpressionContext::GreaterEqual(size_t i) {
  return getToken(TParser::GreaterEqual, i);
}


size_t TParser::RelationalExpressionContext::getRuleIndex() const {
  return TParser::RuleRelationalExpression;
}

void TParser::RelationalExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRelationalExpression(this);
}

void TParser::RelationalExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRelationalExpression(this);
}


std::any TParser::RelationalExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitRelationalExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::RelationalExpressionContext* TParser::relationalExpression() {
  RelationalExpressionContext *_localctx = _tracker.createInstance<RelationalExpressionContext>(_ctx, getState());
  enterRule(_localctx, 94, TParser::RuleRelationalExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(473);
    shiftExpression();
    setState(478);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 74) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 74)) & 15) != 0)) {
      setState(474);
      _la = _input->LA(1);
      if (!(((((_la - 74) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 74)) & 15) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(475);
      shiftExpression();
      setState(480);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ShiftExpressionContext ------------------------------------------------------------------

TParser::ShiftExpressionContext::ShiftExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::AdditiveExpressionContext* TParser::ShiftExpressionContext::additiveExpression() {
  return getRuleContext<TParser::AdditiveExpressionContext>(0);
}


size_t TParser::ShiftExpressionContext::getRuleIndex() const {
  return TParser::RuleShiftExpression;
}

void TParser::ShiftExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterShiftExpression(this);
}

void TParser::ShiftExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitShiftExpression(this);
}


std::any TParser::ShiftExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitShiftExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::ShiftExpressionContext* TParser::shiftExpression() {
  ShiftExpressionContext *_localctx = _tracker.createInstance<ShiftExpressionContext>(_ctx, getState());
  enterRule(_localctx, 96, TParser::RuleShiftExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(481);
    additiveExpression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditiveExpressionContext ------------------------------------------------------------------

TParser::AdditiveExpressionContext::AdditiveExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::MultiplicativeExpressionContext *> TParser::AdditiveExpressionContext::multiplicativeExpression() {
  return getRuleContexts<TParser::MultiplicativeExpressionContext>();
}

TParser::MultiplicativeExpressionContext* TParser::AdditiveExpressionContext::multiplicativeExpression(size_t i) {
  return getRuleContext<TParser::MultiplicativeExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::AdditiveExpressionContext::Plus() {
  return getTokens(TParser::Plus);
}

tree::TerminalNode* TParser::AdditiveExpressionContext::Plus(size_t i) {
  return getToken(TParser::Plus, i);
}

std::vector<tree::TerminalNode *> TParser::AdditiveExpressionContext::Minus() {
  return getTokens(TParser::Minus);
}

tree::TerminalNode* TParser::AdditiveExpressionContext::Minus(size_t i) {
  return getToken(TParser::Minus, i);
}


size_t TParser::AdditiveExpressionContext::getRuleIndex() const {
  return TParser::RuleAdditiveExpression;
}

void TParser::AdditiveExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAdditiveExpression(this);
}

void TParser::AdditiveExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAdditiveExpression(this);
}


std::any TParser::AdditiveExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitAdditiveExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::AdditiveExpressionContext* TParser::additiveExpression() {
  AdditiveExpressionContext *_localctx = _tracker.createInstance<AdditiveExpressionContext>(_ctx, getState());
  enterRule(_localctx, 98, TParser::RuleAdditiveExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(483);
    multiplicativeExpression();
    setState(488);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::Plus

    || _la == TParser::Minus) {
      setState(484);
      _la = _input->LA(1);
      if (!(_la == TParser::Plus

      || _la == TParser::Minus)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(485);
      multiplicativeExpression();
      setState(490);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MultiplicativeExpressionContext ------------------------------------------------------------------

TParser::MultiplicativeExpressionContext::MultiplicativeExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::CastExpressionContext *> TParser::MultiplicativeExpressionContext::castExpression() {
  return getRuleContexts<TParser::CastExpressionContext>();
}

TParser::CastExpressionContext* TParser::MultiplicativeExpressionContext::castExpression(size_t i) {
  return getRuleContext<TParser::CastExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::MultiplicativeExpressionContext::Star() {
  return getTokens(TParser::Star);
}

tree::TerminalNode* TParser::MultiplicativeExpressionContext::Star(size_t i) {
  return getToken(TParser::Star, i);
}

std::vector<tree::TerminalNode *> TParser::MultiplicativeExpressionContext::Slash() {
  return getTokens(TParser::Slash);
}

tree::TerminalNode* TParser::MultiplicativeExpressionContext::Slash(size_t i) {
  return getToken(TParser::Slash, i);
}

std::vector<tree::TerminalNode *> TParser::MultiplicativeExpressionContext::Percent() {
  return getTokens(TParser::Percent);
}

tree::TerminalNode* TParser::MultiplicativeExpressionContext::Percent(size_t i) {
  return getToken(TParser::Percent, i);
}


size_t TParser::MultiplicativeExpressionContext::getRuleIndex() const {
  return TParser::RuleMultiplicativeExpression;
}

void TParser::MultiplicativeExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMultiplicativeExpression(this);
}

void TParser::MultiplicativeExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMultiplicativeExpression(this);
}


std::any TParser::MultiplicativeExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitMultiplicativeExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::MultiplicativeExpressionContext* TParser::multiplicativeExpression() {
  MultiplicativeExpressionContext *_localctx = _tracker.createInstance<MultiplicativeExpressionContext>(_ctx, getState());
  enterRule(_localctx, 100, TParser::RuleMultiplicativeExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(491);
    castExpression();
    setState(496);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (((((_la - 65) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 65)) & 7) != 0)) {
      setState(492);
      _la = _input->LA(1);
      if (!(((((_la - 65) & ~ 0x3fULL) == 0) &&
        ((1ULL << (_la - 65)) & 7) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(493);
      castExpression();
      setState(498);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CastExpressionContext ------------------------------------------------------------------

TParser::CastExpressionContext::CastExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::UnaryExpressionContext* TParser::CastExpressionContext::unaryExpression() {
  return getRuleContext<TParser::UnaryExpressionContext>(0);
}


size_t TParser::CastExpressionContext::getRuleIndex() const {
  return TParser::RuleCastExpression;
}

void TParser::CastExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCastExpression(this);
}

void TParser::CastExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCastExpression(this);
}


std::any TParser::CastExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitCastExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::CastExpressionContext* TParser::castExpression() {
  CastExpressionContext *_localctx = _tracker.createInstance<CastExpressionContext>(_ctx, getState());
  enterRule(_localctx, 102, TParser::RuleCastExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(499);
    unaryExpression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryExpressionContext ------------------------------------------------------------------

TParser::UnaryExpressionContext::UnaryExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::PostfixExpressionContext* TParser::UnaryExpressionContext::postfixExpression() {
  return getRuleContext<TParser::PostfixExpressionContext>(0);
}

tree::TerminalNode* TParser::UnaryExpressionContext::PlusPlus() {
  return getToken(TParser::PlusPlus, 0);
}

TParser::UnaryExpressionContext* TParser::UnaryExpressionContext::unaryExpression() {
  return getRuleContext<TParser::UnaryExpressionContext>(0);
}

tree::TerminalNode* TParser::UnaryExpressionContext::MinusMinus() {
  return getToken(TParser::MinusMinus, 0);
}

TParser::UnaryOperatorContext* TParser::UnaryExpressionContext::unaryOperator() {
  return getRuleContext<TParser::UnaryOperatorContext>(0);
}

TParser::CastExpressionContext* TParser::UnaryExpressionContext::castExpression() {
  return getRuleContext<TParser::CastExpressionContext>(0);
}


size_t TParser::UnaryExpressionContext::getRuleIndex() const {
  return TParser::RuleUnaryExpression;
}

void TParser::UnaryExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnaryExpression(this);
}

void TParser::UnaryExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnaryExpression(this);
}


std::any TParser::UnaryExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitUnaryExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::UnaryExpressionContext* TParser::unaryExpression() {
  UnaryExpressionContext *_localctx = _tracker.createInstance<UnaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 104, TParser::RuleUnaryExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(509);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::Identifier:
      case TParser::IntegerConstant:
      case TParser::FloatingConstant:
      case TParser::CharacterConstant:
      case TParser::StringLiteral:
      case TParser::LeftParen: {
        enterOuterAlt(_localctx, 1);
        setState(501);
        postfixExpression();
        break;
      }

      case TParser::PlusPlus: {
        enterOuterAlt(_localctx, 2);
        setState(502);
        match(TParser::PlusPlus);
        setState(503);
        unaryExpression();
        break;
      }

      case TParser::MinusMinus: {
        enterOuterAlt(_localctx, 3);
        setState(504);
        match(TParser::MinusMinus);
        setState(505);
        unaryExpression();
        break;
      }

      case TParser::Plus:
      case TParser::Minus:
      case TParser::Star:
      case TParser::And:
      case TParser::Tilde:
      case TParser::Not: {
        enterOuterAlt(_localctx, 4);
        setState(506);
        unaryOperator();
        setState(507);
        castExpression();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryOperatorContext ------------------------------------------------------------------

TParser::UnaryOperatorContext::UnaryOperatorContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::UnaryOperatorContext::And() {
  return getToken(TParser::And, 0);
}

tree::TerminalNode* TParser::UnaryOperatorContext::Star() {
  return getToken(TParser::Star, 0);
}

tree::TerminalNode* TParser::UnaryOperatorContext::Plus() {
  return getToken(TParser::Plus, 0);
}

tree::TerminalNode* TParser::UnaryOperatorContext::Minus() {
  return getToken(TParser::Minus, 0);
}

tree::TerminalNode* TParser::UnaryOperatorContext::Tilde() {
  return getToken(TParser::Tilde, 0);
}

tree::TerminalNode* TParser::UnaryOperatorContext::Not() {
  return getToken(TParser::Not, 0);
}


size_t TParser::UnaryOperatorContext::getRuleIndex() const {
  return TParser::RuleUnaryOperator;
}

void TParser::UnaryOperatorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnaryOperator(this);
}

void TParser::UnaryOperatorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnaryOperator(this);
}


std::any TParser::UnaryOperatorContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitUnaryOperator(this);
  else
    return visitor->visitChildren(this);
}

TParser::UnaryOperatorContext* TParser::unaryOperator() {
  UnaryOperatorContext *_localctx = _tracker.createInstance<UnaryOperatorContext>(_ctx, getState());
  enterRule(_localctx, 106, TParser::RuleUnaryOperator);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(511);
    _la = _input->LA(1);
    if (!(((((_la - 63) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 63)) & 807) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PostfixExpressionContext ------------------------------------------------------------------

TParser::PostfixExpressionContext::PostfixExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TParser::PrimaryExpressionContext* TParser::PostfixExpressionContext::primaryExpression() {
  return getRuleContext<TParser::PrimaryExpressionContext>(0);
}

std::vector<tree::TerminalNode *> TParser::PostfixExpressionContext::LeftParen() {
  return getTokens(TParser::LeftParen);
}

tree::TerminalNode* TParser::PostfixExpressionContext::LeftParen(size_t i) {
  return getToken(TParser::LeftParen, i);
}

std::vector<tree::TerminalNode *> TParser::PostfixExpressionContext::RightParen() {
  return getTokens(TParser::RightParen);
}

tree::TerminalNode* TParser::PostfixExpressionContext::RightParen(size_t i) {
  return getToken(TParser::RightParen, i);
}

std::vector<tree::TerminalNode *> TParser::PostfixExpressionContext::PlusPlus() {
  return getTokens(TParser::PlusPlus);
}

tree::TerminalNode* TParser::PostfixExpressionContext::PlusPlus(size_t i) {
  return getToken(TParser::PlusPlus, i);
}

std::vector<tree::TerminalNode *> TParser::PostfixExpressionContext::MinusMinus() {
  return getTokens(TParser::MinusMinus);
}

tree::TerminalNode* TParser::PostfixExpressionContext::MinusMinus(size_t i) {
  return getToken(TParser::MinusMinus, i);
}

std::vector<TParser::ArgumentExpressionListContext *> TParser::PostfixExpressionContext::argumentExpressionList() {
  return getRuleContexts<TParser::ArgumentExpressionListContext>();
}

TParser::ArgumentExpressionListContext* TParser::PostfixExpressionContext::argumentExpressionList(size_t i) {
  return getRuleContext<TParser::ArgumentExpressionListContext>(i);
}


size_t TParser::PostfixExpressionContext::getRuleIndex() const {
  return TParser::RulePostfixExpression;
}

void TParser::PostfixExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPostfixExpression(this);
}

void TParser::PostfixExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPostfixExpression(this);
}


std::any TParser::PostfixExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitPostfixExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::PostfixExpressionContext* TParser::postfixExpression() {
  PostfixExpressionContext *_localctx = _tracker.createInstance<PostfixExpressionContext>(_ctx, getState());
  enterRule(_localctx, 108, TParser::RulePostfixExpression);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(513);
    primaryExpression();
    setState(523);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 6926536226895822848) != 0)) {
      setState(521);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case TParser::LeftParen: {
          setState(514);
          match(TParser::LeftParen);
          setState(516);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (((((_la - 48) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 48)) & 26468415) != 0)) {
            setState(515);
            argumentExpressionList();
          }
          setState(518);
          match(TParser::RightParen);
          break;
        }

        case TParser::PlusPlus: {
          setState(519);
          match(TParser::PlusPlus);
          break;
        }

        case TParser::MinusMinus: {
          setState(520);
          match(TParser::MinusMinus);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(525);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryExpressionContext ------------------------------------------------------------------

TParser::PrimaryExpressionContext::PrimaryExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::PrimaryExpressionContext::Identifier() {
  return getToken(TParser::Identifier, 0);
}

TParser::ConstantContext* TParser::PrimaryExpressionContext::constant() {
  return getRuleContext<TParser::ConstantContext>(0);
}

tree::TerminalNode* TParser::PrimaryExpressionContext::StringLiteral() {
  return getToken(TParser::StringLiteral, 0);
}

tree::TerminalNode* TParser::PrimaryExpressionContext::LeftParen() {
  return getToken(TParser::LeftParen, 0);
}

TParser::ExpressionContext* TParser::PrimaryExpressionContext::expression() {
  return getRuleContext<TParser::ExpressionContext>(0);
}

tree::TerminalNode* TParser::PrimaryExpressionContext::RightParen() {
  return getToken(TParser::RightParen, 0);
}


size_t TParser::PrimaryExpressionContext::getRuleIndex() const {
  return TParser::RulePrimaryExpression;
}

void TParser::PrimaryExpressionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimaryExpression(this);
}

void TParser::PrimaryExpressionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimaryExpression(this);
}


std::any TParser::PrimaryExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitPrimaryExpression(this);
  else
    return visitor->visitChildren(this);
}

TParser::PrimaryExpressionContext* TParser::primaryExpression() {
  PrimaryExpressionContext *_localctx = _tracker.createInstance<PrimaryExpressionContext>(_ctx, getState());
  enterRule(_localctx, 110, TParser::RulePrimaryExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(533);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TParser::Identifier: {
        enterOuterAlt(_localctx, 1);
        setState(526);
        match(TParser::Identifier);
        break;
      }

      case TParser::IntegerConstant:
      case TParser::FloatingConstant:
      case TParser::CharacterConstant: {
        enterOuterAlt(_localctx, 2);
        setState(527);
        constant();
        break;
      }

      case TParser::StringLiteral: {
        enterOuterAlt(_localctx, 3);
        setState(528);
        match(TParser::StringLiteral);
        break;
      }

      case TParser::LeftParen: {
        enterOuterAlt(_localctx, 4);
        setState(529);
        match(TParser::LeftParen);
        setState(530);
        expression();
        setState(531);
        match(TParser::RightParen);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgumentExpressionListContext ------------------------------------------------------------------

TParser::ArgumentExpressionListContext::ArgumentExpressionListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TParser::AssignmentExpressionContext *> TParser::ArgumentExpressionListContext::assignmentExpression() {
  return getRuleContexts<TParser::AssignmentExpressionContext>();
}

TParser::AssignmentExpressionContext* TParser::ArgumentExpressionListContext::assignmentExpression(size_t i) {
  return getRuleContext<TParser::AssignmentExpressionContext>(i);
}

std::vector<tree::TerminalNode *> TParser::ArgumentExpressionListContext::Comma() {
  return getTokens(TParser::Comma);
}

tree::TerminalNode* TParser::ArgumentExpressionListContext::Comma(size_t i) {
  return getToken(TParser::Comma, i);
}


size_t TParser::ArgumentExpressionListContext::getRuleIndex() const {
  return TParser::RuleArgumentExpressionList;
}

void TParser::ArgumentExpressionListContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgumentExpressionList(this);
}

void TParser::ArgumentExpressionListContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgumentExpressionList(this);
}


std::any TParser::ArgumentExpressionListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitArgumentExpressionList(this);
  else
    return visitor->visitChildren(this);
}

TParser::ArgumentExpressionListContext* TParser::argumentExpressionList() {
  ArgumentExpressionListContext *_localctx = _tracker.createInstance<ArgumentExpressionListContext>(_ctx, getState());
  enterRule(_localctx, 112, TParser::RuleArgumentExpressionList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(535);
    assignmentExpression();
    setState(540);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TParser::Comma) {
      setState(536);
      match(TParser::Comma);
      setState(537);
      assignmentExpression();
      setState(542);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstantContext ------------------------------------------------------------------

TParser::ConstantContext::ConstantContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TParser::ConstantContext::IntegerConstant() {
  return getToken(TParser::IntegerConstant, 0);
}

tree::TerminalNode* TParser::ConstantContext::FloatingConstant() {
  return getToken(TParser::FloatingConstant, 0);
}

tree::TerminalNode* TParser::ConstantContext::CharacterConstant() {
  return getToken(TParser::CharacterConstant, 0);
}


size_t TParser::ConstantContext::getRuleIndex() const {
  return TParser::RuleConstant;
}

void TParser::ConstantContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstant(this);
}

void TParser::ConstantContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<TParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstant(this);
}


std::any TParser::ConstantContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TParserVisitor*>(visitor))
    return parserVisitor->visitConstant(this);
  else
    return visitor->visitChildren(this);
}

TParser::ConstantContext* TParser::constant() {
  ConstantContext *_localctx = _tracker.createInstance<ConstantContext>(_ctx, getState());
  enterRule(_localctx, 114, TParser::RuleConstant);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(543);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 3940649673949184) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool TParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 21: return directDeclaratorSempred(antlrcpp::downCast<DirectDeclaratorContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool TParser::directDeclaratorSempred(DirectDeclaratorContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void TParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  tparserParserInitialize();
#else
  ::antlr4::internal::call_once(tparserParserOnceFlag, tparserParserInitialize);
#endif
}
