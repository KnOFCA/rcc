lexer grammar TLexer;

// These are all supported lexer sections:

// Lexer file header. Appears at the top of h + cpp files. Use e.g. for copyrights.
@lexer::header {/* lexer header section */}

// Appears before any #include in h + cpp files.
@lexer::preinclude {/* lexer precinclude section */}

// Follows directly after the standard #includes in h + cpp files.
@lexer::postinclude {
/* lexer postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}

// Directly preceds the lexer class declaration in the h file (e.g. for additional types etc.).
@lexer::context {/* lexer context section */}

// Appears in the public part of the lexer in the h file.
@lexer::members {/* public lexer declarations section */
bool canTestFoo() { return true; }
bool isItFoo() { return true; }
bool isItBar() { return true; }

void myFooLexerAction() { /* do something*/ };
void myBarLexerAction() { /* do something*/ };

/* ============================================================
 *  Lexer code injection (为 C++ 扩展预留)
 * ============================================================ */

bool isCxxEnabled() { return false; }
}

// Appears in the private part of the lexer in the h file.
@lexer::declarations {/* private lexer declarations/members section */}

// Appears in line with the other class member definitions in the cpp file.
@lexer::definitions {/* lexer definitions section */}

channels { CommentsChannel, DirectiveChannel }

tokens {
	DUMMY
}

/* ============================================================
 *  Channels
 * ============================================================ */

channels {
    PREPROCESSOR,
    COMMENTS
}

/* ============================================================
 *  C Keywords (ISO C)
 * ============================================================ */

Auto        : 'auto';
Break       : 'break';
Case        : 'case';
Char        : 'char';
Const       : 'const';
Continue    : 'continue';
Default     : 'default';
Do          : 'do';
Double      : 'double';
Else        : 'else';
Enum        : 'enum';
Extern      : 'extern';
Float       : 'float';
For         : 'for';
Goto        : 'goto';
If          : 'if';
Inline      : 'inline';
Int         : 'int';
Long        : 'long';
Register    : 'register';
Restrict    : 'restrict';
Return      : 'return';
Short       : 'short';
Signed      : 'signed';
Sizeof      : 'sizeof';
Static      : 'static';
Struct      : 'struct';
Switch      : 'switch';
Typedef     : 'typedef';
Union       : 'union';
Unsigned    : 'unsigned';
Void        : 'void';
Volatile    : 'volatile';
While       : 'while';
Alignas     : '_Alignas';
Alignof     : '_Alignof';
Atomic      : '_Atomic';
Bool        : '_Bool';
Complex     : '_Complex';
Generic     : '_Generic';
Imaginary   : '_Imaginary';
Noreturn    : '_Noreturn';
StaticAssert: '_Static_assert';
ThreadLocal : '_Thread_local';

/* ============================================================
 *  C++ reserved words (占位，默认不会匹配)
 * ============================================================ */

CxxClass
    : {isCxxEnabled()}? 'class'
    ;

CxxNamespace
    : {isCxxEnabled()}? 'namespace'
    ;

/* ============================================================
 *  Identifiers
 * ============================================================ */

Identifier
    : IdentifierNondigit
      ( IdentifierNondigit | Digit )*
    ;

fragment IdentifierNondigit
    : [a-zA-Z_]
    ;

fragment Digit
    : [0-9]
    ;

/* ============================================================
 *  Constants
 * ============================================================ */

IntegerConstant
    : DecimalConstant IntegerSuffix?
    ;

fragment DecimalConstant
    : '0'
    | [1-9] Digit*
    ;

fragment IntegerSuffix
    : [uU] [lL]?
    | [lL] [uU]?
    ;

FloatingConstant
    : Digit+ '.' Digit* ExponentPart?
    | '.' Digit+ ExponentPart?
    | Digit+ ExponentPart
    ;

fragment ExponentPart
    : [eE] [+-]? Digit+
    ;

CharacterConstant
    : '\'' ( EscapeSequence | ~['\\\r\n] ) '\''
    ;

StringLiteral
    : '"' ( EscapeSequence | ~["\\\r\n] )* '"'
    ;

/* ============================================================
 *  Escape sequences
 * ============================================================ */

fragment EscapeSequence
    : '\\' [btnvrfa\\'"?]
    | '\\' Digit Digit? Digit?
    | '\\x' HexDigit+
    ;

fragment HexDigit
    : [0-9a-fA-F]
    ;

/* ============================================================
 *  Operators and punctuators
 * ============================================================ */

LeftParen   : '(';
RightParen  : ')';
LeftBracket : '[';
RightBracket: ']';
LeftBrace   : '{';
RightBrace  : '}';
Dot         : '.';
Arrow       : '->';
PlusPlus    : '++';
MinusMinus  : '--';
PlusAssign  : '+=';
MinusAssign : '-=';
MultAssign  : '*=';
DivAssign   : '/=';
ModAssign   : '%=';
AndAssign   : '&=';
OrAssign    : '|=';
XorAssign   : '^=';
LSAssign    : '<<=';
RSAssign    : '>>=';
LeftShift   : '<<';
RightShift  : '>>';
Plus        : '+';
Minus       : '-';
Star        : '*';
Slash       : '/';
Percent     : '%';
And         : '&';
Or          : '|';
Caret       : '^';
Tilde       : '~';
Not         : '!';
Assign      : '=';
Less        : '<';
Greater     : '>';
LessEqual   : '<=';
GreaterEqual: '>=';
Equal       : '==';
NotEqual    : '!=';
AndAnd      : '&&';
OrOr        : '||';
Question    : '?';
Colon       : ':';
Semi        : ';';
Comma       : ',';
Ellipsis    : '...';

/* ============================================================
 *  Preprocessor (kept simple, line-based)
 * ============================================================ */

Preprocessor
    : '#' ~[\r\n]* -> channel(PREPROCESSOR)
    ;

/* ============================================================
 *  Comments
 * ============================================================ */

LineComment
    : '//' ~[\r\n]* -> channel(COMMENTS)
    ;

BlockComment
    : '/*' .*? '*/' -> channel(COMMENTS)
    ;

/* ============================================================
 *  Whitespace
 * ============================================================ */

Whitespace
    : [ \t\r\n]+ -> skip
    ;
