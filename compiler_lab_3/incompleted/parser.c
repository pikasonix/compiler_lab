/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void) {
  Token* tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType) {
  if (lookAhead->tokenType == tokenType) {
    printToken(lookAhead);
    scan();
  } else missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void) {
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void) {
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST) {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  } 
  else compileBlock2();
  assert("Block parsed!");
}

void compileBlock2(void) {
  if (lookAhead->tokenType == KW_TYPE) {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  } 
  else compileBlock3();
}

void compileBlock3(void) {
  if (lookAhead->tokenType == KW_VAR) {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  } 
  else compileBlock4();
}

void compileBlock4(void) {
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void) {
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

// 10) ConstDecls::= ConstDecl ConstDecls
// 11) ConstDecls::= epsilon
void compileConstDecls(void) {
  if (lookAhead->tokenType == TK_IDENT) {
    compileConstDecl();
    compileConstDecls();
  }
}

// 12) ConstDecl ::= Ident SB_EQUAL Constant SB_SEMICOLON
void compileConstDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileConstant();
  eat(SB_SEMICOLON);
}

// 13) TypeDecls ::= TypeDecl TypeDecls
// 14) TypeDecls ::= epsilon
void compileTypeDecls(void) {
  if (lookAhead->tokenType == TK_IDENT) {
    compileTypeDecl();
    compileTypeDecls();
  }
}

// 15) TypeDecl ::= Ident SB_EQUAL Type SB_SEMICOLON
void compileTypeDecl(void) {
  eat(TK_IDENT);
  eat(SB_EQ);
  compileType();
  eat(SB_SEMICOLON);
}

// 16) VarDecls ::= VarDecl VarDecls
// 17) VarDecls ::= epsilon
void compileVarDecls(void) {
  if (lookAhead->tokenType == TK_IDENT) {
    compileVarDecl();
    compileVarDecls();
  }
}

// 18) VarDecl ::= Ident SB_COLON Type SB_SEMICOLON
void compileVarDecl(void) {
  eat(TK_IDENT);
  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
}

// 19) SubDecls ::= FunDecl SubDecls
// 20) SubDecls ::= ProcDecl SubDecls
// 21) SubDecls ::= epsilon
void compileSubDecls(void) {
  assert("Parsing subtoutines ....");
  switch (lookAhead->tokenType) {
  case KW_FUNCTION:
    compileFuncDecl();
    compileSubDecls();
    break;
  case KW_PROCEDURE:
    compileProcDecl();
    compileSubDecls();
    break;
  }
  assert("Subtoutines parsed ....");
}

// FunDecl ::= KW_FUNCTION Ident Params SB_COLON BasicType SB_SEMICOLON Block SB_SEMICOLON
void compileFuncDecl(void) {
  assert("Parsing a function ....");
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
}

// ProcDecl ::= KW_PROCEDURE Ident Params SB_SEMICOLON Block SB_SEMICOLON
void compileProcDecl(void) {
  assert("Parsing a procedure ....");
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
}

// 36) UnsignedConstant ::= Number
// 37) UnsignedConstant ::= ConstIdent
// 38) UnsignedConstant ::= ConstChar
void compileUnsignedConstant(void) {
  switch (lookAhead->tokenType) {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  }
}

// 40) Constant ::= SB_PLUS Constant2
// 41) Constant ::= SB_MINUS Constant2
// 42) Constant ::= Constant2
// 43) Constant ::= ConstChar
void compileConstant(void) {
  switch (lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    compileConstant2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileConstant2();
    break;
  case TK_IDENT:
  case TK_NUMBER:
    compileConstant2();
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  }
}

// 44) Constant2::= ConstIdent
// 45) Constant2::= Number
void compileConstant2(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  }
}

// 30) Type ::= KW_INTEGER
// 31) Type ::= KW_CHAR
// 32) Type ::= TypeIdent
// 33) Type ::= KW_ARRAY SB_LSEL Number SB_RSEL KW_OF Type
void compileType(void) {
  switch (lookAhead->tokenType) {
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case KW_ARRAY:
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);
    eat(SB_RSEL);
    eat(KW_OF);
    compileType();
    break;
  }
}

// 34) BasicType ::= KW_INTEGER
// 35) BasicType ::= KW_CHAR
void compileBasicType(void) {
  switch (lookAhead->tokenType) {
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  }
}

// 24) Params ::= SB_LPAR Param Params2 SB_RPAR
// 25) Params ::= epsilon
void compileParams(void) {
  if (lookAhead->tokenType == SB_LPAR) {
    eat(SB_LPAR);
    compileParam();
    compileParams2();
    eat(SB_RPAR);
  }
}

// 26) Params2 ::= SB_SEMICOLON Param Params2
// 27) Params2 ::= epsilon
void compileParams2(void) {
  if (lookAhead->tokenType == SB_SEMICOLON) {
    eat(SB_SEMICOLON);
    compileParam();
    compileParams2();
  }
}

// 28) Param ::= Ident SB_COLON BasicType
// 29) Param ::= KW_VAR Ident SB_COLON BasicType
void compileParam(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
    break;
  case KW_VAR:
    eat(KW_VAR);
    eat(TK_IDENT);
    eat(SB_COLON);
    compileBasicType();
    break;
  }
}

// 46) Statements ::= Statement Statements2
void compileStatements(void) {
  compileStatement();
  compileStatements2();
}

// 47) Statements2 ::= KW_SEMICOLON Statement Statement2
// 48) Statements2 ::= epsilon
void compileStatements2(void) {
  if (lookAhead->tokenType == SB_SEMICOLON) {
    eat(SB_SEMICOLON);
    compileStatement();
    compileStatements2();
  } else {
    // bổ sung sửa lỗi CALL WRITEI(S) CALL WRITEI(Z); bị thiếu ;
    switch (lookAhead->tokenType) {
    case TK_IDENT:
    case KW_CALL:
    case KW_BEGIN:
    case KW_IF:
    case KW_WHILE:
    case KW_FOR:
      missingToken(SB_SEMICOLON, lookAhead->lineNo, lookAhead->colNo);
      compileStatement();
      compileStatements2();
      break;
    default:
      break;
    }
  }
}

// 49) Statement ::= AssignSt
// 50) Statement ::= CallSt
// 51) Statement ::= GroupSt
// 52) Statement ::= IfSt
// 53) Statement ::= WhileSt
// 54) Statement ::= ForSt
// 55) Statement ::= epsilon
void compileStatement(void) {
  switch (lookAhead->tokenType) {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

// 56) AssignSt ::= Variable SB_ASSIGN Expession
// 57) AssignSt ::= FunctionIdent SB_ASSIGN Expression
void compileAssignSt(void) {
  assert("Parsing an assign statement ....");
  if (lookAhead->tokenType == TK_IDENT) {
    eat(TK_IDENT);
    if (lookAhead->tokenType == SB_LSEL) {
        compileIndexes(); 
    }
    eat(SB_ASSIGN);
    compileExpression();
  }
  assert("Assign statement parsed ....");
}

// 58) CallSt ::= KW_CALL ProcedureIdent Arguments
void compileCallSt(void) {
  assert("Parsing a call statement ....");
  eat(KW_CALL);
  eat(TK_IDENT);
  compileArguments();
  assert("Call statement parsed ....");
}

// 59) GroupSt ::= KW_BEGIN Statements KW_END
void compileGroupSt(void) {
  assert("Parsing a group statement ....");
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

// 60) IfSt ::= KW_IF Condition KW_THEN Statement ElseSt
void compileIfSt(void) {
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE) 
    compileElseSt();
  assert("If statement parsed ....");
}

// 61) ElseSt ::= KW_ELSE statement
// 62) ElseSt ::= epsilon
void compileElseSt(void) {
  eat(KW_ELSE);
  compileStatement();
}

// 63) WhileSt ::= KW_WHILE Condition KW_DO Statement
void compileWhileSt(void) {
  assert("Parsing a while statement ....");
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement parsed ...."); 
}

// 64) ForSt ::= KW_FOR VariableIdent SB_ASSIGN Expression KW_TO Expression KW_DO Statement
void compileForSt(void) {
  assert("Parsing a for statement ....");
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

// 65) Arguments ::= SB_LPAR Expression Arguments2 SB_RPAR
// 66) Arguments ::= epsilon
void compileArguments(void) {
  if (lookAhead->tokenType == SB_LPAR) {
    eat(SB_LPAR);
    compileExpression();
    compileArguments2();
    eat(SB_RPAR);
  }
}

// 67) Arguments2 ::= SB_COMMA Expression Arguments2
// 68) Arguments2 ::= epsilon
void compileArguments2(void) {
  if (lookAhead->tokenType == SB_COMMA) {
    eat(SB_COMMA);
    compileExpression();
    compileArguments2();
  }
}

// 68) Condition ::= Expression Condition2
void compileCondition(void) {
  compileExpression();
  compileCondition2();
}

// 69) Condition2::= SB_EQ Expression
// 70) Condition2::= SB_NEQ Expression
// 71) Condition2::= SB_LE Expression
// 72) Condition2::= SB_LT Expression
// 73) Condition2::= SB_GE Expression
// 74) Condition2::= SB_GT Expression
void compileCondition2(void) {
  switch (lookAhead->tokenType) {
  case SB_EQ:
    eat(SB_EQ);
    compileExpression();
    break;
  case SB_NEQ:
    eat(SB_NEQ);
    compileExpression();
    break;
  case SB_LE:
    eat(SB_LE);
    compileExpression();
    break;
  case SB_LT:
    eat(SB_LT);
    compileExpression();
    break;
  case SB_GE:
    eat(SB_GE);
    compileExpression();
    break;
  case SB_GT:
    eat(SB_GT);
    compileExpression();
    break;
  }
}

// 75) Expression ::= SB_PLUS Expression2
// 76) Expression ::= SB_MINUS Expression2
// 77) Expression ::= Expression2
void compileExpression(void) {
  assert("Parsing an expression");
  switch (lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    compileExpression2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileExpression2();
    break;
  default:
    compileExpression2();
    break;
  }
  assert("Expression parsed");
}

// 78) Expression2 ::= Term Expression3
void compileExpression2(void) {
  compileTerm();
  compileExpression3();
}

// 79) Expression3 ::= SB_PLUS Term Expression3
// 80) Expression3 ::= SB_MINUS Term Expression3
// 81) Expression3 ::= epsilon
void compileExpression3(void) {
  switch (lookAhead->tokenType) {
  case SB_PLUS:
    eat(SB_PLUS);
    compileTerm();
    compileExpression3();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileTerm();
    compileExpression3();
    break;
  }
}

// 82) Term ::= Factor Term2
void compileTerm(void) {
  compileFactor();
  compileTerm2();
}

// 83) Term2 ::= SB_TIMES Factor Term2
// 84) Term2 ::= SB_SLASH Factor Term2
// 85) Term2 ::= epsilon
void compileTerm2(void) {
  switch (lookAhead->tokenType) {
  case SB_TIMES:
    eat(SB_TIMES);
    compileFactor();
    compileTerm2();
    break;
  case SB_SLASH:
    eat(SB_SLASH);
    compileFactor();
    compileTerm2();
    break;
  }
}

// 86) Factor ::= UnsignedConstant
// 87) Factor ::= Variable
// 88) Factor ::= FunctionApptication
// 89) Factor ::= SB_LPAR Expression SB_RPAR
void compileFactor(void) {
  switch (lookAhead->tokenType) {
  case TK_NUMBER:
  case TK_CHAR:
    compileUnsignedConstant();
    break;
  case TK_IDENT:
    eat(TK_IDENT); 
    if (lookAhead->tokenType == SB_LPAR) {
        compileArguments();
    }
    else if (lookAhead->tokenType == SB_LSEL) {
        compileIndexes();
    }
    break;
  case SB_LPAR:
    eat(SB_LPAR);
    compileExpression();
    eat(SB_RPAR);
    break;
  default:
    error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
    scan();
    break;
  }
}

// 92) Indexes ::= SB_LSEL Expression SB_RSEL Indexes
// 93) Indexes ::= epsilon
void compileIndexes(void) {
  if (lookAhead->tokenType == SB_LSEL) {
    eat(SB_LSEL);
    compileExpression();
    eat(SB_RSEL);
    compileIndexes();
  }
}

int compile(char *fileName) {
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;

}
