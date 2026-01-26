/* lexer header section */

// Generated from ANTLR/TLexer.g4 by ANTLR 4.13.2

#pragma once

/* lexer precinclude section */

#include "antlr4-runtime.h"


/* lexer postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif


namespace antlrcpptest {

/* lexer context section */

class  TLexer : public antlr4::Lexer {
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
    CommentsChannel = 2, DirectiveChannel = 3, PREPROCESSOR = 4, COMMENTS = 5
  };

  explicit TLexer(antlr4::CharStream *input);

  ~TLexer() override;

  /* public lexer declarations section */
  bool canTestFoo() { return true; }
  bool isItFoo() { return true; }
  bool isItBar() { return true; }

  void myFooLexerAction() { /* do something*/ };
  void myBarLexerAction() { /* do something*/ };

  /* ============================================================
   *  Lexer code injection (为 C++ 扩展预留)
   * ============================================================ */

  bool isCxxEnabled() { return false; }


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
  /* private lexer declarations/members section */

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.
  bool CxxClassSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);
  bool CxxNamespaceSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);

};

}  // namespace antlrcpptest
