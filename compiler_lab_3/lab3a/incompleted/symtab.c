/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities: quản lý kiểu dữ liệu (int, char, array) ******************************/

//INT
Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

//CHAR
Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

//ARRAY
Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

// ---- Xử lý kiểu dữ liệu ----
// Tạo bản sao cho kiểu dữ liệu: Cấp phát bộ nhớ cho Type mới. Sao chép typeClass. Nếu là mảng (TP_ARRAY), phải sao chép cả arraySize và gọi đệ quy duplicateType cho elementType
Type* duplicateType(Type* type) {
  switch (type->typeClass) {
    case TP_INT: {
      return makeIntType();
    }
    case TP_CHAR: {
      return makeCharType();
    }
    case TP_ARRAY: {
      Type* elemType = duplicateType(type->elementType);
      return makeArrayType(type->arraySize, elemType);
    }
    default:
      return NULL;
  }
}

// So sánh 2 kiểu dữ liệu có giống nhau không: So sánh typeClass. Nếu khác nhau -> trả về 0. Nếu giống nhau và là TP_ARRAY, phải so sánh tiếp arraySize và gọi đệ quy so sánh elementType
int compareType(Type* type1, Type* type2) {
  if (type1->typeClass != type2->typeClass) {
    return 0;
  }
  switch (type1->typeClass) {
    case TP_INT:
    case TP_CHAR:
      return 1;
    case TP_ARRAY:
      return (type1->arraySize == type2->arraySize) && compareType(type1->elementType, type2->elementType);
    default:
      return 0;
  }
}

// Giải phóng bộ nhớ kiểu dữ liệu: Nếu là kiểu mảng, phải giải phóng elementType trước. Sau đó giải phóng chính con trỏ type
void freeType(Type* type) {
  if (type->typeClass == TP_ARRAY) {
    freeType(type->elementType);
  }
  free(type);
}

/******************* Constant utility: quản lý hằng số ******************************/
// INT
ConstantValue* makeIntConstant(int i) {
  ConstantValue* constValue = (ConstantValue*) malloc(sizeof(ConstantValue));
  constValue->type = TP_INT;
  constValue->intValue = i;
  return constValue;
}

// CHAR
ConstantValue* makeCharConstant(char ch) {
  ConstantValue* constValue = (ConstantValue*) malloc(sizeof(ConstantValue));
  constValue->type = TP_CHAR;
  constValue->charValue = ch;
  return constValue;
}

// Tạo bản sao cho hằng số: Cấp phát bộ nhớ cho ConstantValue mới. Sao chép type. Nếu là TP_INT, sao chép intValue; nếu là TP_CHAR, sao chép charValue
ConstantValue* duplicateConstantValue(ConstantValue* v) {
  switch (v->type) {
    case TP_INT: {
      return makeIntConstant(v->intValue);
    }
    case TP_CHAR: {
      return makeCharConstant(v->charValue);
    }
    default:
      return NULL;
  }
}

/******************* Object utilities: quản lý đối tượng (tạo, giải phóng, tìm kiếm) ******************************/
// Tạo scope: Cấp phát bộ nhớ cho Scope mới. Gán objList là NULL, gán owner và outer từ tham số truyền vào
Scope* createScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

// Tạo Object - Program: Cấp phát bộ nhớ cho Object mới. Sao chép tên chương trình vào name. Gán kind là OBJ_PROGRAM. Cấp phát bộ nhớ cho progAttrs và tạo scope mới cho chương trình (không có outer scope). Gán đối tượng chương trình vào symtab->program. Trả về con trỏ đến đối tượng chương trình vừa tạo
Object* createProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program,NULL);
  symtab->program = program;

  return program;
}

// Tạo Object - Constant: Cần cấp phát constAttrs
Object* createConstantObject(char *name) {
  Object* constant = (Object*) malloc(sizeof(Object));
  strcpy(constant->name, name);
  constant->kind = OBJ_CONSTANT;
  constant->constAttrs = (ConstantAttributes*) malloc(sizeof(ConstantAttributes));
  return constant;
}

// Tạo Object - Type: Cần cấp phát typeAttrs
Object* createTypeObject(char *name) {
  Object* typeObj = (Object*) malloc(sizeof(Object));
  strcpy(typeObj->name, name);
  typeObj->kind = OBJ_TYPE;
  typeObj->typeAttrs = (TypeAttributes*) malloc(sizeof(TypeAttributes));
  return typeObj;
}

// Tạo Object - Variable: Cần cấp phát varAttrs
Object* createVariableObject(char *name) {
  Object* variable = (Object*) malloc(sizeof(Object));
  strcpy(variable->name, name);
  variable->kind = OBJ_VARIABLE;
  variable->varAttrs = (VariableAttributes*) malloc(sizeof(VariableAttributes));
  variable->varAttrs->scope = symtab->currentScope;
  return variable;
}

// Tạo Object - Function: Cần cấp phát funcAttrs và tạo scope mới cho hàm (outer scope là phạm vi hiện tại)
Object* createFunctionObject(char *name) {
  Object* function = (Object*) malloc(sizeof(Object));
  strcpy(function->name, name);
  function->kind = OBJ_FUNCTION;
  function->funcAttrs = (FunctionAttributes*) malloc(sizeof(FunctionAttributes));
  function->funcAttrs->scope = createScope(function, symtab->currentScope);
  function->funcAttrs->paramList = NULL;
  // chưa cần khởi tạo luôn returnType vì... (lý do theo //TODO lab3b )
  function->funcAttrs->returnType = NULL;
  
  return function;
}

// Tạo Object - Procedure: Cần cấp phát procAttrs và tạo scope mới cho thủ tục (outer scope là phạm vi hiện tại)
Object* createProcedureObject(char *name) {
  Object* procedure = (Object*) malloc(sizeof(Object));
  strcpy(procedure->name, name);
  procedure->kind = OBJ_PROCEDURE;
  procedure->procAttrs = (ProcedureAttributes*) malloc(sizeof(ProcedureAttributes));
  procedure->procAttrs->scope = createScope(procedure, symtab->currentScope);
  procedure->procAttrs->paramList = NULL;
  
  return procedure;
}

// Tạo Object - Parameter: Cần cấp phát paramAttrs, gán function là owner
Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  Object* parameter = (Object*) malloc(sizeof(Object));
  strcpy(parameter->name, name);
  parameter->kind = OBJ_PARAMETER;
  parameter->paramAttrs = (ParameterAttributes*) malloc(sizeof(ParameterAttributes));
  parameter->paramAttrs->kind = kind;
  parameter->paramAttrs->function = owner;
  
  return parameter;
}

// Giải phóng bộ nhớ Object: giải phóng các thuộc tính (constAttrs, varAttrs, typeAttrs, funcAttrs, procAttrs, progAttrs, paramAttrs). Sau đó giải phóng con trỏ obj
void freeObject(Object* obj) {
  switch (obj->kind) {
    case OBJ_CONSTANT:
      free(obj->constAttrs->value);
      free(obj->constAttrs);
      break;
    case OBJ_VARIABLE:
      freeType(obj->varAttrs->type);
      free(obj->varAttrs);
      break;
    case OBJ_TYPE:
      freeType(obj->typeAttrs->actualType);
      free(obj->typeAttrs);
      break;
    case OBJ_FUNCTION:
      freeReferenceList(obj->funcAttrs->paramList);
      freeType(obj->funcAttrs->returnType);
      freeScope(obj->funcAttrs->scope);
      free(obj->funcAttrs);
      break;
    case OBJ_PROCEDURE:
      freeReferenceList(obj->procAttrs->paramList);
      freeScope(obj->procAttrs->scope);
      free(obj->procAttrs);
      break;
    case OBJ_PROGRAM:
      freeScope(obj->progAttrs->scope);
      free(obj->progAttrs);
      break;
    case OBJ_PARAMETER:
      freeType(obj->paramAttrs->type);
      free(obj->paramAttrs);
      break;
  }
  free(obj);
}

// Giải phóng bộ nhớ Scope: giải phóng Object List. Sau đó giải phóng con trỏ scope
void freeScope(Scope* scope) {
  freeObjectList(scope->objList);
  free(scope);
}

// Giải phóng Object List: duyệt qua từng ObjectNode trong objList, gọi freeObject cho từng object, sau đó giải phóng từng ObjectNode
void freeObjectList(ObjectNode *objList) {
  ObjectNode* current = objList;
  while (current != NULL) {
    ObjectNode* next = current->next;
    freeObject(current->object);
    free(current);
    current = next;
  }
}

// Giải phóng Reference List: duyệt qua từng ObjectNode trong objList, chỉ giải phóng từng ObjectNode (không giải phóng object bên trong)
void freeReferenceList(ObjectNode *objList) {
  ObjectNode* current = objList;
  while (current != NULL) {
    ObjectNode* next = current->next;
    free(current);
    current = next;
  }
}

// Thêm Object vào Object List: tạo ObjectNode mới, gán object và next=NULL. Nếu objList rỗng, gán objList là node mới. Ngược lại, duyệt đến cuối objList và thêm node mới vào cuối
void addObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

// Tìm kiếm Object trong Object List theo tên: duyệt qua objList, so sánh tên. Nếu tìm thấy, trả về con trỏ đến Object; nếu không tìm thấy, trả về NULL
Object* findObject(ObjectNode *objList, char *name) {
  ObjectNode* current = objList;
  while (current != NULL) {
    if (strcmp(current->object->name, name) == 0) {
      return current->object;
    }
    current = current->next;
  }
  return NULL;
}

/******************* others: quản lý trạng thái toàn cục của symbol table (khởi tạo, vào/ra, phạm vi, khai báo) ******************************/
// Khởi tạo bảng ký hiệu: cấp phát bộ nhớ cho symtab, gán globalObjectList là NULL. Tạo các đối tượng hàm/procedure chuẩn: READC, READI, WRITEI, WRITEC, WRITELN. Khai báo các đối tượng này vào globalObjectList. Tạo và gán intType, charType
void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList),param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

// Giải phóng bảng ký hiệu: giải phóng Program-Object, Object List toàn cục, symtab, intType, charType
void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

// Vào scope mới: gán currentScope thành scope truyền vào
void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

// Ra khỏi scope: gán currentScope thành outer scope của currentScope
void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

// Khai báo đối tượng vào bảng ký hiệu: nếu đối tượng là tham số, phải thêm nó vào danh sách tham số của hàm/procedure sở hữu. Sau đó thêm đối tượng vào danh sách đối tượng của phạm vi hiện tại
void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  addObject(&(symtab->currentScope->objList), obj);
}


