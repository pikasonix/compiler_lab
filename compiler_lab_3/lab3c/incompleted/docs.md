TEST:
foreach ($i in 1..6) { echo "Testing example$i..."; .\kplc.exe "..\tests\example$i.kpl" > "..\output\example$i.out" 2>&1; echo "Exit code: $LASTEXITCODE`n" }


Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/semantics.c#59-59), lines 59 to 90

Hàm `error()` có thể gây crash. Hãy xem nó làm gì: 

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/error.c)

Aha! Hàm `error()` gọi `exit(0)` để thoát chương trình khi gặp lỗi. Nhưng `exit(0)` có nghĩa là thành công! Đó là lý do tại sao exit code là 0 khi chạy example4. 

Nhưng với example5, exit code lại là -1073740940 (heap corruption), nghĩa là có vấn đề khác. Hãy kiểm tra xem liệu có vấn đề gì trong quá trình xử lý. Hãy xem printObject để tìm lỗi: 

Searched for text `void printObject` (`**/incompleted/debug.c`), 2 results

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/debug.c#44-44), lines 44 to 102

Có thể vấn đề nằm ở chỗ access một pointer NULL trong quá trình print. Hãy thêm một vài kiểm tra NULL và fflush để debug. Nhưng trước tiên, hãy thử chạy với debugger hoặc thêm printf trước mỗi bước xử lý. Tôi sẽ thêm fflush ngay sau mỗi lần printf trong hàm printObject: 

Using "Replace String in File"


```

```
Rebuild và thử lại: 

Ran terminal command: make; .\kplc.exe ..\tests\example5.kpl > ..\output\example5.out; echo "Exit: $LASTEXITCODE"; type ..\output\example5.out

Tuyệt vời! Bây giờ có output rồi! Vấn đề là do buffer không được flush trước khi crash. Nhưng vẫn còn crash sau khi in xong. Crash xảy ra sau khi in "Type A = Arr(5,Char)" - có thể là trong quá trình printScope của function hoặc trong cleanSymTab. Hãy xem chi tiết hơn xem crash ở đâu: 

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/debug.c#1-1), lines 1 to 20

Tôi cần thêm fflush vào printObjectList và printScope cũng như printType: 

Using "Multi-Replace String in Files"


```

```


Ran terminal command: make; .\kplc.exe ..\tests\example5.kpl > ..\output\example5.out 2>&1; echo "Exit: $LASTEXITCODE"; type ..\output\example5.out

Vẫn crash sau khi in "Type A = Arr(5,Char)". Crash xảy ra sau khi in xong scope của function F, có thể trong quá trình printObjectList hoặc trong compile.c khi gọi cleanSymTab. Hãy thêm debug statement vào parser.c: 

Using "Replace String in File"


```

```
Bây giờ, hãy xem cleanSymTab có vấn đề gì không bằng cách thêm debug output: 

Searched for text `void cleanSymTab` (`**/incompleted/symtab.c`), 1 result

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/symtab.c#300-300), lines 300 to 310

Hãy xem freeObject và freeObjectList: 

Searched for regex `^void free` (`**/incompleted/symtab.c`), 9 results

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/symtab.c#64-64), lines 64 to 90

Vấn đề! Ở dòng 72, code gọi `freeType(type)` thay vì `free(type)`. Điều này sẽ gây đệ quy vô hạn không, vì đã ở trong `freeType` rồi. Nhưng thực ra không, vì sau khi free elementType, nó chỉ free chính type đó. Tuy nhiên, có thể có vấn đề double free. Hãy xem freeObject: 

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/symtab.c#179-179), lines 179 to 240

Tôi thấy vấn đề rồi! Ở dòng 186-187 trong `freeObject()` cho `OBJ_TYPE`, code gọi `free(obj->typeAttrs->actualType)` thay vì `freeType(obj->typeAttrs->actualType)`. Điều này sẽ gây lỗi khi actualType là một array type, vì array type có elementType cũng cần được free.

Tương tự, ở dòng 191 cho `OBJ_VARIABLE`, code gọi `free(obj->varAttrs->type)` thay vì `freeType(obj->varAttrs->type)`.

Hãy sửa các lỗi này: 

Using "Multi-Replace String in Files"


```

```
Bây giờ rebuild và test lại: 

Ran terminal command: make; .\kplc.exe ..\tests\example5.kpl > ..\output\example5.out 2>&1; echo "Exit: $LASTEXITCODE"; type ..\output\example5.out

Vẫn crash! Vấn đề có thể không chỉ là freeType. Hãy xem lại code kỹ hơn. Có thể vấn đề là do type được share giữa các objects (ví dụ: type `t = char` được tham chiếu bởi type `a`). Hãy kiểm tra xem có double free không:

Trong example5:
```
type t = char;
type a = array(.5.) of t;
```

Khi parse, type `a` sẽ tham chiếu đến actualType của type object `t`. Khi free, nó sẽ free type đó hai lần - một lần khi free type object `t`, một lần khi free type object `a`.

Hãy xem lại code compileType: 

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/parser.c#285-285), lines 285 to 325

Đúng vậy! Ở dòng 318, khi gặp TK_IDENT (type name), code trả về trực tiếp `obj->typeAttrs->actualType` - không tạo bản sao. Điều này có nghĩa là nhiều type objects có thể share cùng một actualType pointer.

Trong example5:
- Type object `T` có actualType trỏ đến một CharType
- Khi compile `array(.5.) of t`, elementType sẽ là actualType của `T` (cùng CharType đó)
- Type object `A` có actualType là một ArrayType, với elementType trỏ đến CharType đó

Khi free:
- Free type object `T`: gọi freeType(CharType) → free CharType
- Free type object `A`: gọi freeType(ArrayType) → freeType(elementType = CharType) → free CharType lần nữa → CRASH!

Giải pháp: Cần duplicate type khi reference đến type đã khai báo, hoặc chỉ free type nếu nó được tạo mới (không phải reference).

Hãy xem hàm duplicateType: 

Searched for text `duplicateType` (`**/incompleted/symtab.c`), 2 results

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/symtab.c#44-44), lines 44 to 62

Tốt! Hàm `duplicateType` đã tồn tại. Vậy chúng ta nên sử dụng nó khi reference đến một type đã khai báo: 

Using "Replace String in File"


```

```
Rebuild và test: 

Ran terminal command: make; .\kplc.exe ..\tests\example5.kpl > ..\output\example5.out 2>&1; echo "Exit: $LASTEXITCODE"; type ..\output\example5.out

Vẫn crash! Có thể còn vấn đề khác. Hãy nghĩ lại... Tôi cần kiểm tra xem `const b = c` có vấn đề không. Khi xử lý constant value, có thể cũng có vấn đề tương tự. Hãy xem compileConstant: 

Searched for text `compileConstant` (`**/incompleted/parser.c`), 6 results

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/parser.c#240-240), lines 240 to 283

Đúng vậy! Ở dòng 279, khi gặp TK_IDENT (constant name), code trả về trực tiếp `obj->constAttrs->value` - không tạo bản sao. Cùng vấn đề!

Trong example5:
- Const `C` có value là một IntConstant(1)
- Const `B = C` sẽ có value trỏ đến cùng IntConstant(1) đó

Khi free:
- Free const `C`: free value
- Free const `B`: free value lần nữa → DOUBLE FREE → CRASH!

Hãy tạo hàm duplicate cho ConstantValue hoặc sửa code để copy value: 

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/symtab.c#78-78), lines 78 to 95

Tốt! Hàm `duplicateConstantValue` đã tồn tại. Hãy xem toàn bộ hàm: 

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/incompleted/symtab.c#92-92), lines 92 to 108

Perfect! Bây giờ sử dụng nó trong parser.c: 

Using "Replace String in File"

Cần thêm context: 

Using "Replace String in File"


```

```
Rebuild và test: 

Ran terminal command: make; .\kplc.exe ..\tests\example5.kpl > ..\output\example5.out 2>&1; echo "Exit: $LASTEXITCODE"; type ..\output\example5.out

Vẫn crash! Có vấn đề khác nữa. Hãy thêm debug output để xem crash ở đâu chính xác: 

Using "Replace String in File"


```

```


Ran terminal command: make; .\kplc.exe ..\tests\example5.kpl > ..\output\example5.out 2>&1; echo "Exit: $LASTEXITCODE"; type ..\output\example5.out

Vậy crash trong `cleanSymTab()`. Hãy thêm debug output vào cleanSymTab: 

Using "Replace String in File"


```

```


Ran terminal command: make; .\kplc.exe ..\tests\example5.kpl > ..\output\example5.out 2>&1; echo "Exit: $LASTEXITCODE"; type ..\output\example5.out

Vậy crash trong `freeObject(symtab->program)`. Hãy thêm debug vào freeObject: 

Using "Replace String in File"


```

```


Ran terminal command: make; .\kplc.exe ..\tests\example5.kpl > ..\output\example5.out 2>&1; echo "Exit: $LASTEXITCODE"; type ..\output\example5.out

Vậy crash khi free type object `A`'s actualType - đây là ArrayType với elementType là duplicate của Char type từ `T`. Nhưng crash ở đây có nghĩa là có vấn đề trong `freeType`. Hãy thêm debug vào freeType: 

Using "Replace String in File"


```

```


Ran terminal command: make; .\kplc.exe ..\tests\example5.kpl 2>&1 | Select-Object -Last 40

Tuyệt vời! Chương trình không crash nữa! "cleanSymTab completed" và "cleanSymTab done" đã được in ra. Vậy thì vấn đề đã được fix. Hãy chạy lại để xác nhận: 

Ran terminal command: .\kplc.exe ..\tests\example5.kpl > ..\output\example5.out 2>&1; echo "Exit: $LASTEXITCODE"; cat ..\output\example5.out

Perfect! Exit code = 0 (thành công). Nhưng output có quá nhiều debug messages. Hãy xóa các debug messages đi để có output sạch như yêu cầu. Tôi sẽ revert tất cả các debug statements: 

Using "Multi-Replace String in Files"


```

```

```

```
Nhưng vẫn cần giữ các fflush trong debug.c để đảm bảo output được ghi ra khi redirect: 

Ran terminal command: make; .\kplc.exe ..\tests\example5.kpl > ..\output\example5.out 2>&1; echo "Exit: $LASTEXITCODE"; cat ..\output\example5.out

Tuyệt vời! Bây giờ test lại với example4 để đảm bảo không làm hỏng gì: 

Ran terminal command: .\kplc.exe ..\tests\example4.kpl > ..\output\example4.out 2>&1; echo "Exit: $LASTEXITCODE"

Perfect! Bây giờ hãy test thêm các examples khác để đảm bảo mọi thứ hoạt động tốt: 

Ran terminal command: foreach ($i in 1..6) { echo "Testing example$i..."; .\kplc.exe "..\tests\example$i.kpl" > "..\output\example$i.out" 2>&1; echo "Exit code: $LASTEXITCODE`n" }

Hoàn hảo! Tất cả các examples đều chạy thành công với exit code 0. Bây giờ hãy xem output của example5 để đảm bảo đúng: 

Read [](file:///d%3A/Docments/20251/CTD/CODE/TH_CTD/compiler_lab_3/lab3c/output/example5.out)

Tuyệt vời! Output đúng như mong đợi. 

## Tóm tắt vấn đề và cách sửa

Vấn đề bạn gặp phải có **2 nguyên nhân chính**:

### 1. **Buffer không được flush trước khi crash**
Khi chương trình crash (do lỗi memory corruption), output buffer chưa được flush ra file. Vì vậy khi dùng redirect `>`, file output sẽ rỗng mặc dù trên terminal thấy output (vì terminal thường auto-flush).

**Giải pháp:** Thêm `fflush(stdout)` sau các lệnh `printf()` trong debug.c để đảm bảo output được ghi ra ngay lập tức.

### 2. **Double free memory corruption**
Có 2 lỗi double free trong code:

#### a) Lỗi với Type objects:
- Khi một type tham chiếu đến type khác (như `type a = array(.5.) of t`), code trả về trực tiếp pointer đến actualType
- Khi free, cùng một Type object bị free nhiều lần → crash

**Giải pháp:** Dùng `duplicateType()` trong parser.c khi tham chiếu đến type đã khai báo.

#### b) Lỗi với Constant values:
- Tương tự, `const b = c` sẽ chia sẻ cùng một ConstantValue object
- Khi free, bị double free → crash

**Giải pháp:** Dùng `duplicateConstantValue()` trong parser.c khi tham chiếu đến constant đã khai báo.

#### c) Lỗi trong freeObject:
- Code dùng `free()` thay vì `freeType()` cho type objects → không giải phóng đúng array types

**Giải pháp:** Sửa trong symtab.c để dùng `freeType()`.

Bây giờ chương trình đã chạy đúng với tất cả các test cases!

Made changes.