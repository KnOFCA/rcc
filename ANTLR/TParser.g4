parser grammar TParser;

options {
	tokenVocab = TLexer;
}

// These are all supported parser sections:

// Parser file header. Appears at the top in all parser related files. Use e.g. for copyrights.
@parser::header {/* parser/listener/visitor header section */}

// Appears before any #include in h + cpp files.
@parser::preinclude {/* parser precinclude section */}

// Follows directly after the standard #includes in h + cpp files.
@parser::postinclude {
/* parser postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}

// Directly preceeds the parser class declaration in the h file (e.g. for additional types etc.).
@parser::context {/* parser context section */}

// Appears in the private part of the parser in the h file.
// The function bodies could also appear in the definitions section, but I want to maximize
// Java compatibility, so we can also create a Java parser from this grammar.
// Still, some tweaking is necessary after the Java file generation (e.g. bool -> boolean).
@parser::members {
/* public parser declarations/members section */
bool myAction() { return true; }
bool doesItBlend() { return true; }
void cleanUp() {}
void doInit() {}
void doAfter() {}
}

// Appears in the public part of the parser in the h file.
@parser::declarations {/* private parser declarations section */}

// Appears in line with the other class member definitions in the cpp file.
@parser::definitions {/* parser definitions section */}

// Additionally there are similar sections for (base)listener and (base)visitor files.
@parser::listenerpreinclude {/* listener preinclude section */}
@parser::listenerpostinclude {/* listener postinclude section */}
@parser::listenerdeclarations {/* listener public declarations/members section */}
@parser::listenermembers {/* listener private declarations/members section */}
@parser::listenerdefinitions {/* listener definitions section */}

@parser::baselistenerpreinclude {/* base listener preinclude section */}
@parser::baselistenerpostinclude {/* base listener postinclude section */}
@parser::baselistenerdeclarations {/* base listener public declarations/members section */}
@parser::baselistenermembers {/* base listener private declarations/members section */}
@parser::baselistenerdefinitions {/* base listener definitions section */}

@parser::visitorpreinclude {/* visitor preinclude section */}
@parser::visitorpostinclude {/* visitor postinclude section */}
@parser::visitordeclarations {/* visitor public declarations/members section */}
@parser::visitormembers {/* visitor private declarations/members section */}
@parser::visitordefinitions {/* visitor definitions section */}

@parser::basevisitorpreinclude {/* base visitor preinclude section */}
@parser::basevisitorpostinclude {/* base visitor postinclude section */}
@parser::basevisitordeclarations {/* base visitor public declarations/members section */}
@parser::basevisitormembers {/* base visitor private declarations/members section */}
@parser::basevisitordefinitions {/* base visitor definitions section */}

// Actual grammar start.

/* ============================================================
 *  Translation unit
 * ============================================================ */

translationUnit
    : externalDeclaration*
    ;

/* ============================================================
 *  External declarations
 * ============================================================ */

externalDeclaration
    : functionDefinition
    | declaration
    ;

/* ============================================================
 *  Function definition
 * ============================================================ */

functionDefinition
    : declarationSpecifiers declarator compoundStatement
    ;

/* ============================================================
 *  Declarations
 * ============================================================ */

declaration
    : declarationSpecifiers initDeclaratorList? Semi
    ;

declarationSpecifiers
    : declarationSpecifier+
    ;

declarationSpecifier
    : storageClassSpecifier
    | typeSpecifier
    | typeQualifier
    ;

storageClassSpecifier
    : Auto
    | Register
    | Static
    | Extern
    | Typedef
    ;

typeSpecifier
    : Void
    | Char
    | Short
    | Int
    | Long
    | Float
    | Double
    | Signed
    | Unsigned
    | Bool
    | structSpecifier
    | unionSpecifier
    | enumSpecifier
    ;

typeQualifier
    : Const
    | Volatile
    | Restrict
    | Atomic
    ;

/* ============================================================
 *  Struct / Union / Enum
 * ============================================================ */

structSpecifier
    : Struct Identifier? LeftBrace structDeclarationList RightBrace
    ;

unionSpecifier
    : Union Identifier? LeftBrace structDeclarationList RightBrace
    ;

structDeclarationList
    : structDeclaration+
    ;

structDeclaration
    : declarationSpecifiers structDeclaratorList Semi
    ;

structDeclaratorList
    : structDeclarator (Comma structDeclarator)*
    ;

structDeclarator
    : declarator
    ;

enumSpecifier
    : Enum Identifier? LeftBrace enumeratorList RightBrace
    ;

enumeratorList
    : enumerator (Comma enumerator)* Comma?
    ;

enumerator
    : Identifier (Assign constantExpression)?
    ;

/* ============================================================
 *  Declarators
 * ============================================================ */

initDeclaratorList
    : initDeclarator (Comma initDeclarator)*
    ;

initDeclarator
    : declarator (Assign initializer)?
    ;

declarator
    : pointer? directDeclarator
    ;

directDeclarator
    : Identifier
    | LeftParen declarator RightParen
    | directDeclarator LeftParen parameterTypeList? RightParen
    ;

pointer
    : Star typeQualifier* pointer?
    ;

parameterTypeList
    : parameterList
    ;

parameterList
    : parameterDeclaration (Comma parameterDeclaration)*
    ;

parameterDeclaration
    : declarationSpecifiers declarator
    ;

/* ============================================================
 *  Initializers
 * ============================================================ */

initializer
    : assignmentExpression
    | LeftBrace initializerList RightBrace
    ;

initializerList
    : initializer (Comma initializer)* Comma?
    ;

/* ============================================================
 *  Statements
 * ============================================================ */

statement
    : labeledStatement
    | compoundStatement
    | expressionStatement
    | selectionStatement
    | iterationStatement
    | jumpStatement
    ;

labeledStatement
    : Identifier Colon statement          // 普通标签
    | Case constantExpression Colon statement
    | Default Colon statement
    ;

compoundStatement
    : LeftBrace blockItemList? RightBrace
    ;

blockItemList
    : blockItem+
    ;

blockItem
    : declaration
    | statement
    ;

expressionStatement
    : expression? Semi
    ;

selectionStatement
    : If LeftParen expression RightParen statement (Else statement)?
    | Switch LeftParen expression RightParen statement
    ;

iterationStatement
    : While LeftParen expression RightParen statement
    | Do statement While LeftParen expression RightParen Semi
    | For LeftParen expressionStatement expressionStatement expression? RightParen statement
    ;

jumpStatement
    : Goto Identifier Semi
    | Continue Semi
    | Break Semi
    | Return expression? Semi
    ;

/* ============================================================
 *  Expressions
 * ============================================================ */

expression
    : assignmentExpression (Comma assignmentExpression)*
    ;

assignmentExpression
    : conditionalExpression
    | unaryExpression Assign assignmentExpression
    ;

conditionalExpression
    : logicalOrExpression
      (Question expression Colon conditionalExpression)?
    ;

constantExpression
    : conditionalExpression
    ;

logicalOrExpression
    : logicalAndExpression (OrOr logicalAndExpression)*
    ;

logicalAndExpression
    : inclusiveOrExpression (AndAnd inclusiveOrExpression)*
    ;

inclusiveOrExpression
    : exclusiveOrExpression (Or exclusiveOrExpression)*
    ;

exclusiveOrExpression
    : andExpression (Caret andExpression)*
    ;

andExpression
    : equalityExpression (And equalityExpression)*
    ;

equalityExpression
    : relationalExpression
      ((Equal | NotEqual) relationalExpression)*
    ;

relationalExpression
    : shiftExpression
      ((Less | Greater | LessEqual | GreaterEqual) shiftExpression)*
    ;

shiftExpression
    : additiveExpression
    ;

additiveExpression
    : multiplicativeExpression
      ((Plus | Minus) multiplicativeExpression)*
    ;

multiplicativeExpression
    : castExpression
      ((Star | Slash | Percent) castExpression)*
    ;

castExpression
    : unaryExpression
    ;

unaryExpression
    : postfixExpression
    | PlusPlus unaryExpression
    | MinusMinus unaryExpression
    | unaryOperator castExpression
    ;

unaryOperator
    : And
    | Star
    | Plus
    | Minus
    | Tilde
    | Not
    ;

postfixExpression
    : primaryExpression
      (   LeftParen argumentExpressionList? RightParen
        | PlusPlus
        | MinusMinus
      )*
    ;

primaryExpression
    : Identifier
    | constant
    | StringLiteral
    | LeftParen expression RightParen
    ;

argumentExpressionList
    : assignmentExpression (Comma assignmentExpression)*
    ;

/* ============================================================
 *  Constants
 * ============================================================ */

constant
    : IntegerConstant
    | FloatingConstant
    | CharacterConstant
    ;
