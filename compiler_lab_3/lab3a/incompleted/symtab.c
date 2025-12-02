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
  // TODO
}

// So sánh 2 kiểu dữ liệu có giống nhau không: So sánh typeClass. Nếu khác nhau -> trả về 0. Nếu giống nhau và là TP_ARRAY, phải so sánh tiếp arraySize và gọi đệ quy so sánh elementType
int compareType(Type* type1, Type* type2) {
  // TODO
}

// Giải phóng bộ nhớ kiểu dữ liệu: Nếu là kiểu mảng, phải giải phóng elementType trước. Sau đó giải phóng chính con trỏ type
void freeType(Type* type) {
  // TODO
}

/******************* Constant utility: quản lý hằng số ******************************/
// INT
ConstantValue* makeIntConstant(int i) {
  // TODO
}

// CHAR
ConstantValue* makeCharConstant(char ch) {
  // TODO
}

// Tạo bản sao cho hằng số: Cấp phát bộ nhớ cho ConstantValue mới. Sao chép type. Nếu là TP_INT, sao chép intValue; nếu là TP_CHAR, sao chép charValue
ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
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
  // TODO
}

// Tạo Object - Type: Cần cấp phát typeAttrs
Object* createTypeObject(char *name) {
  // TODO
}

// Tạo Object - Variable: Cần cấp phát varAttrs
Object* createVariableObject(char *name) {
  // TODO
}

// Tạo Object - Function: Cần cấp phát funcAttrs và tạo scope mới cho hàm (outer scope là phạm vi hiện tại)
Object* createFunctionObject(char *name) {
  // TODO
}

// Tạo Object - Procedure: Cần cấp phát procAttrs và tạo scope mới cho thủ tục (outer scope là phạm vi hiện tại)
Object* createProcedureObject(char *name) {
  // TODO
}

// Tạo Object - Parameter: Cần cấp phát paramAttrs, gán function là owner
Object* createParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
}

// Giải phóng bộ nhớ Object: giải phóng các thuộc tính (constAttrs, varAttrs, typeAttrs, funcAttrs, procAttrs, progAttrs, paramAttrs). Sau đó giải phóng con trỏ obj
void freeObject(Object* obj) {
  // TODO
}

// Giải phóng bộ nhớ Scope: giải phóng Object List. Sau đó giải phóng con trỏ scope
void freeScope(Scope* scope) {
  // TODO
}

// Giải phóng Object List: duyệt qua từng ObjectNode trong objList, gọi freeObject cho từng object, sau đó giải phóng từng ObjectNode
void freeObjectList(ObjectNode *objList) {
  // TODO
}

// Giải phóng Reference List: duyệt qua từng ObjectNode trong objList, chỉ giải phóng từng ObjectNode (không giải phóng object bên trong)
void freeReferenceList(ObjectNode *objList) {
  // TODO
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
  // TODO
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


