/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab* symtab;
extern Token* currentToken;

// Tìm kiếm một object (biến/hàm/procedure/constant/type) trong bảng ký hiệu (Cần tìm từ scope hiện tại lên các scope cha)
// [lab3b] tìm kiếm Object (bản pro của findObject) theo tên trong phạm vi hiện tại và các phạm vi bao ngoài
// Object* findObject(ObjectNode *objList, char *name) {
//   while (objList != NULL) {
//     if (strcmp(objList->object->name, name) == 0) 
//       return objList->object;
//     else objList = objList->next;
//   }
//   return NULL;
// }

Object* lookupObject(char *name) {
  // scope hiện tại
  Scope* scope = symtab->currentScope;
  Object* obj;
  
  // Tìm kiếm (scope hiện tại -> scope cha)
  while (scope != NULL) {
    obj = findObject(scope->objList, name);
    if (obj != NULL) return obj;
    scope = scope->outer;
  }
  
  // Tìm trong globalObjectList (built-in functions/procedures)
  obj = findObject(symtab->globalObjectList, name);
  return obj;
}

// Kiểm tra identifier có "fresh" (chưa được khai báo) trong block hiện tại hay không (Nếu đã tồn tại -> báo lỗi)
void checkFreshIdent(char *name) {
  Object* obj = findObject(symtab->currentScope->objList, name);
  if (obj != NULL) 
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
}

// Kiểm tra identifier đã được khai báo hay chưa (dùng cho các trường hợp sử dụng identifier)
Object* checkDeclaredIdent(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) 
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  return obj;
}

// Kiểm tra Constant đã được khai báo hay chưa (OBJECT_CONSTANT)
Object* checkDeclaredConstant(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) 
    error(ERR_UNDECLARED_CONSTANT, currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_CONSTANT)
    error(ERR_INVALID_CONSTANT, currentToken->lineNo, currentToken->colNo);
  return obj;
}

// Kiểm tra Type đã được khai báo hay chưa (OBJECT_TYPE)
Object* checkDeclaredType(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) 
    error(ERR_UNDECLARED_TYPE, currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_TYPE)
    error(ERR_INVALID_TYPE, currentToken->lineNo, currentToken->colNo);
  return obj;
}

// Kiểm tra Variable đã được khai báo hay chưa (OBJECT_VARIABLE)
Object* checkDeclaredVariable(char* name) {
  Object* obj = lookupObject(name);
  
  if (obj == NULL) 
    error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_VARIABLE)
    error(ERR_INVALID_VARIABLE, currentToken->lineNo, currentToken->colNo);
  return obj;
}

// Kiểm tra Function đã được khai báo hay chưa (OBJECT_FUNCTION)
Object* checkDeclaredFunction(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) 
    error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_FUNCTION)
    error(ERR_INVALID_FUNCTION, currentToken->lineNo, currentToken->colNo);
  return obj;
}

// Kiểm tra Procedure đã được khai báo hay chưa (OBJECT_PROCEDURE)
Object* checkDeclaredProcedure(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) 
    error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  if (obj->kind != OBJ_PROCEDURE)
    error(ERR_INVALID_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  return obj;
}

// Kiểm tra LValue Identifier (vế trái phép gán) đã được khai báo hay chưa (variable, parameter, function)
Object* checkDeclaredLValueIdent(char* name) {
  Object* obj = checkDeclaredIdent(name);
  if ((obj->kind != OBJ_VARIABLE) && (obj->kind != OBJ_PARAMETER) && (obj->kind != OBJ_FUNCTION))
    error(ERR_INVALID_LVALUE, currentToken->lineNo, currentToken->colNo);
  return obj;
}

