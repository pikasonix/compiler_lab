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
Object* lookupObject(char *name) {
  // TODO

}

// Kiểm tra identifier có "fresh" (chưa được khai báo) trong block hiện tại hay không (Nếu đã tồn tại -> báo lỗi)
void checkFreshIdent(char *name) {
  // TODO
}

// Kiểm tra identifier đã được khai báo hay chưa (dùng cho các trường hợp sử dụng identifier)
Object* checkDeclaredIdent(char* name) {
  // TODO
}

// Kiểm tra Constant đã được khai báo hay chưa (OBJECT_CONSTANT)
Object* checkDeclaredConstant(char* name) {
  // TODO
}

// Kiểm tra Type đã được khai báo hay chưa (OBJECT_TYPE)
Object* checkDeclaredType(char* name) {
  // TODO
}

// Kiểm tra Variable đã được khai báo hay chưa (OBJECT_VARIABLE)
Object* checkDeclaredVariable(char* name) {
  // TODO
}

// Kiểm tra Function đã được khai báo hay chưa (OBJECT_FUNCTION)
Object* checkDeclaredFunction(char* name) {
  // TODO
}

// Kiểm tra Procedure đã được khai báo hay chưa (OBJECT_PROCEDURE)
Object* checkDeclaredProcedure(char* name) {
  // TODO
}

// Kiểm tra LValue Identifier (vế trái phép gán) đã được khai báo hay chưa (variable, parameter, function)
Object* checkDeclaredLValueIdent(char* name) {
  // TODO
}

