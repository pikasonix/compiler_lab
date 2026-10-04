# Tài liệu Ngôn ngữ KPL (K Programming Language)

## Giới thiệu
KPL là một ngôn ngữ lập trình giáo dục có cú pháp tương tự Pascal, được sử dụng trong các bài thực hành về trình biên dịch.

---

## Cấu trúc chương trình cơ bản

```kpl
PROGRAM TenChuongTrinh;

(* --- Phần khai báo --- *)
CONST
  (* Khai báo hằng số *)

TYPE
  (* Định nghĩa kiểu dữ liệu mới *)

VAR
  (* Khai báo biến toàn cục *)

(* --- Khai báo chương trình con (hàm, thủ tục) --- *)
FUNCTION ...
PROCEDURE ...

(* --- Thân chương trình chính --- *)
BEGIN
  (* Các câu lệnh sẽ được thực thi ở đây *)
END.
```

**Lưu ý:**
- Tên chương trình, hằng, biến, hàm, thủ tục **không phân biệt** chữ hoa/chữ thường
- Chương trình **phải** kết thúc bằng `END.` (có dấu chấm)
- Từ khóa có thể viết hoa hoặc thường: `PROGRAM`, `Program`, `program` đều hợp lệ

---

## Chú thích

```kpl
(* Đây là một chú thích trên một dòng *)

(*
  Đây là một chú thích
  trên nhiều dòng.
*)
```

**Lưu ý:** Chú thích được đặt trong cặp `(* ... *)`

---

## Kiểu dữ liệu cơ bản

### INTEGER
- Số nguyên **có dấu** (không phải không dấu như mô tả cũ)
- Ví dụ: `0`, `123`, `-456`, `1000`

### CHAR
- Một ký tự duy nhất, được đặt trong cặp dấu nháy đơn `' '`
- Ví dụ: `'a'`, `'Z'`, `'5'`, `' '` (ký tự khoảng trắng)
- **Chuỗi ký tự nhiều ký tự** cũng được hỗ trợ trong một số ngữ cảnh: `'abc'`

---

## Khai báo hằng số (CONST)

```kpl
PROGRAM ViDuHang;

CONST
  MAX_SIZE = 100;
  MIN_VALUE = -10;    (* Hằng số âm *)
  OFFSET = +5;        (* Hằng số dương có dấu + *)
  YES = 1;
  NO = 0;
  CHAR_A = 'A';
  BASE = MAX_SIZE;    (* Tham chiếu hằng số khác *)

BEGIN
  (* Sử dụng hằng số *)
END.
```

**Lưu ý:**
- Hằng số phải được gán giá trị cụ thể khi khai báo
- Hằng số có thể có dấu `+` hoặc `-` ở đầu (với số nguyên)
- Hằng số có thể **tham chiếu** đến hằng số khác đã khai báo trước đó
- Không thể thay đổi giá trị hằng sau khi khai báo

---

## Khai báo biến (VAR)

```kpl
PROGRAM ViDuKhaiBao;

VAR
  tuoi : INTEGER;
  diem : INTEGER;
  kyTuDauTien : CHAR;

BEGIN
  tuoi := 25;
  kyTuDauTien := 'A';
  diem := tuoi + 5;
END.
```

**Lưu ý:**
- Phép gán sử dụng `:=` (không phải `=`)
- Mỗi biến phải khai báo kiểu dữ liệu
- Mỗi dòng khai báo **một biến**: `VAR a : INTEGER;` (không thể khai báo nhiều biến cùng lúc như Pascal)

---

## Định nghĩa kiểu dữ liệu mới (TYPE)

### Mảng 1 chiều

```kpl
PROGRAM ViDuMang1Chieu;

TYPE
  Mang1Chieu = ARRAY (. 100 .) OF INTEGER;  (* Mảng 100 phần tử *)

VAR
  danhSachDiem : Mang1Chieu;
  i, temp : INTEGER;

BEGIN
  (* Gán giá trị - chỉ số bắt đầu từ 0 *)
  danhSachDiem(0) := 10;
  danhSachDiem(1) := 9;
  
  (* Truy cập mảng *)
  temp := danhSachDiem(0); (* temp = 10 *)
  
  (* Sử dụng biến làm chỉ số *)
  i := 1;
  temp := danhSachDiem(i); (* temp = 9 *)
END.
```

### Mảng 2 chiều

```kpl
PROGRAM ViDuMang2Chieu;

TYPE
  MaTranVuong = ARRAY (. 50 .) OF ARRAY (. 50 .) OF INTEGER; (* Mảng 50x50 *)

VAR
  maTranA : MaTranVuong;
  i, j, temp : INTEGER;

BEGIN
  (* Gán giá trị *)
  maTranA(0)(0) := 5;
  maTranA(0)(1) := 10;
  
  (* Truy cập với biến *)
  i := 0;
  j := 1;
  temp := maTranA(i)(j); (* temp = 10 *)
END.
```

**Lưu ý quan trọng về mảng:**
- Cú pháp khai báo: `ARRAY (. kích_thước .) OF kiểu_dữ_liệu`
  - `kích_thước` phải là **số nguyên dương** (TK_NUMBER), không thể là biểu thức hoặc hằng
- Cú pháp truy cập: `tenMang(chi_so)` (dùng dấu ngoặc tròn `()`, không phải ngoặc vuông `[]`)
  - `chi_so` có thể là biểu thức: số, biến, hoặc phép tính
- Chỉ số mảng **bắt đầu từ 0** (không phải từ 1)
- `ARRAY (. 100 .)` tạo mảng có 100 phần tử với chỉ số từ 0 đến 99

---

## Vào/Ra dữ liệu (I/O)

### Hàm đọc dữ liệu (trả về giá trị)

| Hàm | Mô tả | Cách dùng |
|-----|-------|-----------|
| `READI` | Đọc và trả về một số INTEGER | `x := READI;` |
| `READC` | Đọc và trả về một CHAR | `c := READC;` |

### Thủ tục ghi dữ liệu (dùng CALL)

| Thủ tục | Mô tả | Cách dùng |
|---------|-------|-----------|
| `WRITEI(x)` | In ra một giá trị INTEGER | `CALL WRITEI(100);` |
| `WRITEC(c)` | In ra một giá trị CHAR | `CALL WRITEC('A');` |
| `WRITELN` | In ký tự xuống dòng | `CALL WRITELN;` |

### Ví dụ đầy đủ

```kpl
PROGRAM ViDuIO;

VAR
  soA, soB, tong : INTEGER;
  kyTu : CHAR;

BEGIN
  (* Đọc hai số nguyên *)
  soA := READI;
  soB := READI;
  
  (* Tính tổng và in ra *)
  tong := soA + soB;
  CALL WRITEI(tong);
  CALL WRITELN;
  
  (* Đọc và in ký tự *)
  kyTu := READC;
  CALL WRITEC(kyTu);
  CALL WRITELN;
END.
```

**Lưu ý:**
- `READI` và `READC` là **hàm** nên dùng trong phép gán
- `WRITEI`, `WRITEC`, `WRITELN` là **thủ tục** nên phải dùng với từ khóa `CALL`
- KPL **không hỗ trợ** in chuỗi thông báo trực tiếp như `"Nhap so: "`

---

## Toán tử và biểu thức

### Toán tử số học

| Toán tử | Ý nghĩa | Ví dụ |
|---------|---------|-------|
| `+` | Cộng | `a + b` |
| `-` | Trừ | `a - b` |
| `*` | Nhân | `a * b` |
| `/` | Chia | `a / b` |

### Toán tử so sánh

| Toán tử | Ý nghĩa | Ví dụ |
|---------|---------|-------|
| `=` | Bằng | `a = b` |
| `!=` | Khác | `a != b` |
| `<` | Nhỏ hơn | `a < b` |
| `>` | Lớn hơn | `a > b` |
| `<=` | Nhỏ hơn hoặc bằng | `a <= b` |
| `>=` | Lớn hơn hoặc bằng | `a >= b` |

**Lưu ý:**
- Phép so sánh "khác" dùng `!=` (không phải `<>` như Pascal)
- Phép so sánh "bằng" dùng `=` (một dấu bằng)
- Phép gán dùng `:=` (hai ký tự)

---

## Câu lệnh điều kiện IF-THEN-ELSE

### Cú pháp cơ bản

```kpl
IF điều_kiện THEN
  câu_lệnh;
```

### IF-THEN-ELSE

```kpl
IF điều_kiện THEN
  câu_lệnh_1
ELSE
  câu_lệnh_2;
```

**Lưu ý quan trọng về dấu chấm phẩy:**
- **KHÔNG** có dấu `;` sau câu lệnh trước `ELSE`
- **CÓ** dấu `;` sau câu lệnh cuối cùng của cấu trúc IF

### Ví dụ

```kpl
PROGRAM ViDuIF;

VAR
  a, b, max : INTEGER;

BEGIN
  a := READI;
  b := READI;
  
  (* Ví dụ 1: IF đơn giản *)
  IF a > b THEN
    max := a;
  
  (* Ví dụ 2: IF-ELSE *)
  IF a > b THEN
    CALL WRITEI(a)  (* Không có ; ở đây *)
  ELSE
    CALL WRITEI(b);
  
  (* Ví dụ 3: IF với khối BEGIN-END *)
  IF a != 0 THEN
  BEGIN
    CALL WRITEI(a);
    CALL WRITELN;
  END;
  
  (* Ví dụ 4: IF-ELSE với khối BEGIN-END *)
  IF a > b THEN
  BEGIN
    max := a;
    CALL WRITEI(max);
  END  (* Không có ; trước ELSE *)
  ELSE
  BEGIN
    max := b;
    CALL WRITEI(max);
  END;
END.
```

---

## Vòng lặp FOR

### Cú pháp

```kpl
FOR biến := giá_trị_đầu TO giá_trị_cuối DO
  câu_lệnh;
```

### Ví dụ

```kpl
PROGRAM ViDuFOR;

VAR
  i, tong : INTEGER;

BEGIN
  (* Tính tổng từ 1 đến 10 *)
  tong := 0;
  FOR i := 1 TO 10 DO
    tong := tong + i;
  
  CALL WRITEI(tong);  (* Kết quả: 55 *)
  CALL WRITELN;
  
  (* FOR với khối BEGIN-END *)
  FOR i := 1 TO 5 DO
  BEGIN
    CALL WRITEI(i);
    CALL WRITELN;
  END;
END.
```

**Lưu ý:**
- Biến đếm `i` phải được khai báo trong phần VAR
- Chỉ hỗ trợ vòng lặp tăng dần (TO), không có DOWNTO
- Giá trị đầu và giá trị cuối có thể là biến hoặc biểu thức

---

## Vòng lặp WHILE

### Cú pháp

```kpl
WHILE điều_kiện DO
  câu_lệnh;
```

### Ví dụ

```kpl
PROGRAM ViDuWHILE;

VAR
  i, tong : INTEGER;

BEGIN
  (* In các số từ 1 đến 10 *)
  i := 1;
  WHILE i <= 10 DO
  BEGIN
    CALL WRITEI(i);
    CALL WRITELN;
    i := i + 1;
  END;
  
  (* Tính tổng các số chẵn từ 2 đến 100 *)
  i := 2;
  tong := 0;
  WHILE i <= 100 DO
  BEGIN
    tong := tong + i;
    i := i + 2;
  END;
  
  CALL WRITEI(tong);
END.
```

**Lưu ý:**
- Phải khởi tạo giá trị biến điều kiện trước vòng lặp
- Phải cập nhật biến điều kiện bên trong vòng lặp để tránh vòng lặp vô hạn

---

## Hàm (FUNCTION)

### Cú pháp

```kpl
FUNCTION TenHam(tham_so_1: Kiểu_1; tham_so_2: Kiểu_2; ...) : Kiểu_trả_về;
VAR
  (* Biến cục bộ nếu cần *)
BEGIN
  (* Thân hàm *)
  TenHam := giá_trị_trả_về;  (* Gán giá trị cho tên hàm *)
END;
```

### Ví dụ

```kpl
PROGRAM ViDuFunction;

VAR
  a, b, ketqua : INTEGER;

(* Hàm tính giá trị lớn hơn *)
FUNCTION Max(x: INTEGER; y: INTEGER) : INTEGER;
BEGIN
  IF x > y THEN
    Max := x  (* Gán giá trị trả về cho tên hàm *)
  ELSE
    Max := y;
END;

(* Hàm tính giai thừa *)
FUNCTION Factorial(n : INTEGER) : INTEGER;
BEGIN
  IF n = 0 THEN 
    Factorial := 1 
  ELSE 
    Factorial := n * Factorial(n - 1);
END;

BEGIN 
  a := READI;
  b := READI;
  
  (* Gọi hàm trong biểu thức *)
  ketqua := Max(a, b);
  CALL WRITEI(ketqua);
  CALL WRITELN;
  
  (* Gọi hàm lồng nhau *)
  ketqua := Max(Factorial(3), Factorial(4));
  CALL WRITEI(ketqua);
END.
```

**Lưu ý:**
- Hàm **phải** trả về giá trị bằng cách gán cho tên hàm
- Hàm có thể đệ quy (gọi chính nó)
- Hàm được gọi trong biểu thức, không cần từ khóa CALL

---

## Thủ tục (PROCEDURE)

### Cú pháp

```kpl
PROCEDURE TenThuTuc(tham_so_1: Kiểu_1; tham_so_2: Kiểu_2; ...);
VAR
  (* Biến cục bộ nếu cần *)
BEGIN
  (* Thân thủ tục *)
END;
```

### Ví dụ

```kpl
PROGRAM ViDuProcedure;

VAR
  a, b : INTEGER;

(* Thủ tục in tổng *)
PROCEDURE InTong(so1: INTEGER; so2: INTEGER);
VAR
  tong : INTEGER;
BEGIN
  tong := so1 + so2;
  CALL WRITEI(tong);
  CALL WRITELN;
END;

(* Thủ tục in bảng cửu chương *)
PROCEDURE BangCuuChuong(n: INTEGER);
VAR
  i : INTEGER;
BEGIN
  FOR i := 1 TO 10 DO
  BEGIN
    CALL WRITEI(n * i);
    CALL WRITELN;
  END;
END;

BEGIN 
  a := READI;
  b := READI;
  
  (* Gọi thủ tục bằng từ khóa CALL *)
  CALL InTong(a, b);
  CALL BangCuuChuong(5);
END.
```

**Lưu ý:**
- Thủ tục **không** trả về giá trị
- Thủ tục **phải** được gọi bằng từ khóa `CALL`
- Thủ tục có thể gọi thủ tục khác hoặc đệ quy

---

## Tham số truyền theo tham chiếu (VAR Parameter)

KPL hỗ trợ truyền tham số theo tham chiếu (reference parameter) bằng cách sử dụng từ khóa `VAR` trong khai báo tham số.

### Cú pháp

```kpl
PROCEDURE TenThuTuc(VAR tham_so: Kiểu);
(* hoặc *)
FUNCTION TenHam(VAR tham_so: Kiểu) : KieuTraVe;
```

### Ví dụ

```kpl
PROGRAM ViDuVarParameter;

VAR
  x, y : INTEGER;

(* Thủ tục hoán đổi giá trị hai biến *)
PROCEDURE Swap(VAR a: INTEGER; VAR b: INTEGER);
VAR
  temp : INTEGER;
BEGIN
  temp := a;
  a := b;
  b := temp;
END;

(* Thủ tục tăng giá trị biến *)
PROCEDURE Increment(VAR x: INTEGER);
BEGIN
  x := x + 1;
END;

BEGIN
  x := 5;
  y := 10;
  
  CALL WRITEI(x);  (* In ra: 5 *)
  CALL WRITEI(y);  (* In ra: 10 *)
  CALL WRITELN;
  
  (* Hoán đổi giá trị *)
  CALL Swap(x, y);
  
  CALL WRITEI(x);  (* In ra: 10 *)
  CALL WRITEI(y);  (* In ra: 5 *)
  CALL WRITELN;
  
  (* Tăng giá trị *)
  CALL Increment(x);
  CALL WRITEI(x);  (* In ra: 11 *)
END.
```

**Lưu ý quan trọng:**
- Tham số `VAR` cho phép thay đổi giá trị biến gốc
- Khi gọi với tham số `VAR`, **phải** truyền vào biến (không được truyền hằng số hoặc biểu thức)
- Kiểu dữ liệu của đối số phải khớp chính xác với kiểu tham số

### Sự khác biệt giữa tham số thường và VAR

```kpl
(* Tham số thường - truyền theo giá trị *)
PROCEDURE Test1(x: INTEGER);
BEGIN
  x := x + 1;  (* Chỉ thay đổi bản sao *)
END;

(* Tham số VAR - truyền theo tham chiếu *)
PROCEDURE Test2(VAR x: INTEGER);
BEGIN
  x := x + 1;  (* Thay đổi biến gốc *)
END;

VAR a : INTEGER;
BEGIN
  a := 5;
  CALL Test1(a);
  CALL WRITEI(a);  (* In ra: 5 - không thay đổi *)
  
  CALL Test2(a);
  CALL WRITEI(a);  (* In ra: 6 - đã thay đổi *)
END.
```

---

## Quy tắc phạm vi (Scope Rules)

### Biến cục bộ và toàn cục

```kpl
PROGRAM ViDuScope;

VAR
  x : INTEGER;  (* Biến toàn cục *)

PROCEDURE Test;
VAR
  x : INTEGER;  (* Biến cục bộ, che khuất biến toàn cục *)
BEGIN
  x := 10;  (* Gán cho biến cục bộ *)
END;

BEGIN
  x := 5;   (* Gán cho biến toàn cục *)
  CALL Test;
  CALL WRITEI(x);  (* In ra: 5 - biến toàn cục không đổi *)
END.
```

**Lưu ý:**
- Biến khai báo trong FUNCTION/PROCEDURE là biến cục bộ
- Biến cục bộ có thể che khuất biến toàn cục cùng tên
- Biến cục bộ chỉ tồn tại trong thân hàm/thủ tục

---

## Thứ tự khai báo

Trong KPL, các phần khai báo phải tuân theo thứ tự sau:

1. **CONST** - Khai báo hằng số
2. **TYPE** - Định nghĩa kiểu dữ liệu
3. **VAR** - Khai báo biến
4. **FUNCTION/PROCEDURE** - Khai báo hàm và thủ tục
5. **BEGIN...END** - Thân chương trình chính

```kpl
PROGRAM DungThuTu;

(* 1. CONST *)
CONST
  MAX = 100;

(* 2. TYPE *)
TYPE
  Mang = ARRAY (. 100 .) OF INTEGER;

(* 3. VAR *)
VAR
  arr : Mang;
  i : INTEGER;

(* 4. FUNCTION/PROCEDURE *)
FUNCTION Test : INTEGER;
BEGIN
  Test := 0;
END;

(* 5. BEGIN...END *)
BEGIN
  i := Test;
END.
```

---

## Một số lưu ý quan trọng

### 1. Từ khóa và tên định danh
- Từ khóa không phân biệt hoa/thường: `BEGIN`, `Begin`, `begin` đều giống nhau
- Tên biến, hàm, thủ tục không phân biệt hoa/thường: `myVar` và `MYVAR` là một

### 2. Dấu chấm phẩy
- Dấu `;` ngăn cách các câu lệnh
- **Không** có `;` trước `ELSE`
- **Không** có `;` trước `END` (nhưng có thể có)

### 3. Gọi hàm vs thủ tục
- **Hàm**: gọi trực tiếp trong biểu thức: `x := Max(a, b);`
- **Thủ tục**: phải dùng `CALL`: `CALL PrintSum(a, b);`

### 4. Mảng
- Chỉ số mảng bắt đầu từ **0**
- Truy cập bằng `()`: `arr(0)`, `matrix(i)(j)`

### 5. Đệ quy
- KPL hỗ trợ đệ quy cho cả hàm và thủ tục
- Cần có điều kiện dừng để tránh tràn stack

### 6. Giới hạn
- Không hỗ trợ chuỗi ký tự (string) như kiểu dữ liệu
- Không có kiểu boolean (dùng INTEGER: 0 = false, khác 0 = true)
- Không có toán tử logic AND, OR, NOT (dùng biểu thức số học)

---

## Ví dụ chương trình hoàn chỉnh

### Chương trình tính Fibonacci

```kpl
PROGRAM Fibonacci;

VAR
  n, i : INTEGER;

FUNCTION Fib(num : INTEGER) : INTEGER;
BEGIN
  IF num <= 1 THEN
    Fib := num
  ELSE
    Fib := Fib(num - 1) + Fib(num - 2);
END;

BEGIN
  n := READI;
  
  FOR i := 0 TO n DO
  BEGIN
    CALL WRITEI(Fib(i));
    CALL WRITELN;
  END;
END.
```

### Chương trình Tower of Hanoi

```kpl
PROGRAM TowerOfHanoi;

VAR
  count, n : INTEGER;

PROCEDURE Hanoi(disks: INTEGER; source: INTEGER; dest: INTEGER);
VAR
  aux : INTEGER;
BEGIN
  IF disks != 0 THEN
  BEGIN
    aux := 6 - source - dest;
    CALL Hanoi(disks - 1, source, aux);
    
    count := count + 1;
    CALL WRITEI(count);
    CALL WRITEI(disks);
    CALL WRITEI(source);
    CALL WRITEI(dest);
    CALL WRITELN;
    
    CALL Hanoi(disks - 1, aux, dest);
  END;
END;

BEGIN
  n := READI;
  count := 0;
  CALL Hanoi(n, 1, 3);
END.
```

### Chương trình tính tổng số chẵn

```kpl
PROGRAM TongSoChan;

VAR
  n, i, tong : INTEGER;

BEGIN
  (* Đọc số lượng phần tử *)
  n := READI;
  
  (* Khởi tạo tổng *)
  tong := 0;
  
  (* Tính tổng các số chẵn từ 0 đến n *)
  i := 0;
  WHILE i <= n DO
  BEGIN
    tong := tong + i;
    i := i + 2;  (* Chỉ lấy số chẵn *)
  END;
  
  (* In kết quả *)
  CALL WRITEI(tong);
  CALL WRITELN;
END.
```

### Chương trình tính tổng số lẻ

```kpl
PROGRAM TongSoLe;

VAR
  n, i, tong : INTEGER;

BEGIN
  (* Đọc số lượng phần tử *)
  n := READI;
  
  (* Khởi tạo tổng *)
  tong := 0;
  
  (* Tính tổng các số lẻ từ 1 đến n *)
  i := 1;
  WHILE i <= n DO
  BEGIN
    tong := tong + i;
    i := i + 2;  (* Chỉ lấy số lẻ *)
  END;
  
  (* In kết quả *)
  CALL WRITEI(tong);
  CALL WRITELN;
END.
```

### Chương trình tính tổng số chẵn và lẻ (sử dụng hàm)

```kpl
PROGRAM TongChanLe;

VAR
  n, tongChan, tongLe : INTEGER;

(* Hàm kiểm tra số chẵn *)
FUNCTION LaSoChan(x : INTEGER) : INTEGER;
VAR
  remainder : INTEGER;
BEGIN
  (* Tính x mod 2 bằng cách lấy x - (x/2)*2 *)
  remainder := x - (x / 2) * 2;
  
  IF remainder = 0 THEN
    LaSoChan := 1  (* True - là số chẵn *)
  ELSE
    LaSoChan := 0;  (* False - là số lẻ *)
END;

(* Thủ tục tính tổng số chẵn và lẻ *)
PROCEDURE TinhTongChanLe(n : INTEGER; VAR sumChan : INTEGER; VAR sumLe : INTEGER);
VAR
  i : INTEGER;
BEGIN
  sumChan := 0;
  sumLe := 0;
  
  FOR i := 1 TO n DO
  BEGIN
    IF LaSoChan(i) = 1 THEN
      sumChan := sumChan + i
    ELSE
      sumLe := sumLe + i;
  END;
END;

BEGIN
  (* Đọc giá trị n *)
  n := READI;
  
  (* Tính tổng số chẵn và lẻ *)
  CALL TinhTongChanLe(n, tongChan, tongLe);
  
  (* In kết quả *)
  CALL WRITEI(tongChan);
  CALL WRITELN;
  CALL WRITEI(tongLe);
  CALL WRITELN;
END.
```

---

## Xử lý lỗi thường gặp

### 1. Thiếu dấu chấm phẩy
```kpl
(* SAI *)
x := 5
y := 10;

(* ĐÚNG *)
x := 5;
y := 10;
```

### 2. Dấu chấm phẩy trước ELSE
```kpl
(* SAI *)
IF x > 0 THEN
  y := 1;  (* Không được có ; ở đây *)
ELSE
  y := 0;

(* ĐÚNG *)
IF x > 0 THEN
  y := 1
ELSE
  y := 0;
```

### 3. Gọi thủ tục không có CALL
```kpl
(* SAI *)
PrintSum(a, b);

(* ĐÚNG *)
CALL PrintSum(a, b);
```

### 4. Quên khai báo biến
```kpl
(* SAI *)
BEGIN
  x := 5;  (* x chưa được khai báo *)
END.

(* ĐÚNG *)
VAR x : INTEGER;
BEGIN
  x := 5;
END.
```

### 5. Sai kiểu dữ liệu tham số
```kpl
(* SAI *)
PROCEDURE Test(VAR x: INTEGER);
BEGIN
  x := x + 1;
END;

VAR c : CHAR;
BEGIN
  CALL Test(c);  (* c là CHAR nhưng tham số là INTEGER *)
END.
```

### 6. Truyền giá trị cho tham số VAR
```kpl
(* SAI *)
PROCEDURE Increment(VAR x: INTEGER);
BEGIN
  x := x + 1;
END;

BEGIN
  CALL Increment(5);  (* Không được truyền hằng số cho VAR parameter *)
END.

(* ĐÚNG *)
VAR a : INTEGER;
BEGIN
  a := 5;
  CALL Increment(a);  (* Phải truyền biến *)
END.
```

---

## Tổng kết

KPL là ngôn ngữ đơn giản nhưng đầy đủ các khái niệm cơ bản của lập trình:
- Biến và kiểu dữ liệu
- Cấu trúc điều khiển (IF, FOR, WHILE)
- Mảng
- Hàm và thủ tục
- Đệ quy
- Tham số truyền theo giá trị và tham chiếu

KPL là công cụ tốt để học về thiết kế trình biên dịch và các khái niệm ngôn ngữ lập trình.

---

## Các điểm khác biệt so với Pascal

### 1. Cú pháp mảng
- **Pascal**: `ARRAY [1..100] OF INTEGER` và truy cập `arr[i]`
- **KPL**: `ARRAY (. 100 .) OF INTEGER` và truy cập `arr(i)` (chỉ số từ 0)

### 2. So sánh khác
- **Pascal**: `<>`
- **KPL**: `!=`

### 3. Phép gán
- **Pascal và KPL**: cùng dùng `:=`

### 4. Khai báo biến
- **Pascal**: Có thể khai báo nhiều biến cùng kiểu: `VAR a, b, c : INTEGER;`
- **KPL**: Mỗi dòng chỉ khai báo một biến

### 5. Gọi thủ tục
- **Pascal**: Không cần từ khóa đặc biệt
- **KPL**: Phải dùng từ khóa `CALL`

### 6. Vòng lặp FOR
- **Pascal**: Hỗ trợ cả TO và DOWNTO
- **KPL**: Chỉ hỗ trợ TO (tăng dần)

### 7. Kích thước mảng
- **Pascal**: Có thể dùng hằng số hoặc khoảng: `ARRAY [1..MAX]`
- **KPL**: Chỉ dùng số nguyên: `ARRAY (. 100 .)`

---

## Từ khóa (Keywords) của KPL

```
PROGRAM    BEGIN      END        CONST      TYPE       VAR
INTEGER    CHAR       ARRAY      OF         FUNCTION   PROCEDURE
IF         THEN       ELSE       WHILE      DO         FOR
TO         CALL
```

---

## Ký hiệu đặc biệt (Special Symbols)

| Ký hiệu | Tên trong BNF | Ý nghĩa |
|---------|---------------|---------|
| `;` | SB_SEMICOLON | Kết thúc câu lệnh/khai báo |
| `.` | SB_PERIOD | Kết thúc chương trình |
| `:` | SB_COLON | Phân cách tên và kiểu |
| `:=` | SB_ASSIGN | Phép gán |
| `=` | SB_EQ | So sánh bằng |
| `!=` | SB_NEQ | So sánh khác |
| `<` | SB_LT | Nhỏ hơn |
| `<=` | SB_LE | Nhỏ hơn hoặc bằng |
| `>` | SB_GT | Lớn hơn |
| `>=` | SB_GE | Lớn hơn hoặc bằng |
| `+` | SB_PLUS | Cộng/Dấu dương |
| `-` | SB_MINUS | Trừ/Dấu âm |
| `*` | SB_TIMES | Nhân |
| `/` | SB_SLASH | Chia |
| `(` | SB_LPAR | Ngoặc trái |
| `)` | SB_RPAR | Ngoặc phải |
| `(.` | SB_LSEL | Mở chỉ số mảng |
| `.)` | SB_RSEL | Đóng chỉ số mảng |
| `,` | SB_COMMA | Phân cách tham số |

---

## Câu lệnh rỗng (Empty Statement)

KPL cho phép câu lệnh rỗng (empty statement). Điều này hữu ích khi cần cú pháp yêu cầu một câu lệnh nhưng không cần thực hiện gì:

```kpl
(* Câu lệnh IF không làm gì khi điều kiện đúng *)
IF x > 0 THEN
  (* Câu lệnh rỗng - không cần viết gì *)
ELSE
  x := 0;

(* Hoặc dùng dấu chấm phẩy trực tiếp *)
IF x > 0 THEN
  ;  (* Câu lệnh rỗng *)
ELSE
  x := 0;
```

---

## Phạm vi kiểu dữ liệu (Type System)

### Kiểu cơ bản (BasicType)
- Chỉ có `INTEGER` và `CHAR`
- Dùng cho tham số hàm/thủ tục và kiểu trả về của hàm

### Kiểu mở rộng (Type)
- Bao gồm: `INTEGER`, `CHAR`, tên kiểu đã định nghĩa (TK_IDENT), và `ARRAY`
- Dùng cho khai báo biến và định nghĩa kiểu mới

### Ví dụ minh họa

```kpl
TYPE
  MyArray = ARRAY (. 10 .) OF INTEGER;
  
VAR
  arr : MyArray;  (* OK - Type có thể là TK_IDENT *)

(* SAI - Không thể dùng kiểu mảng trực tiếp làm tham số *)
FUNCTION Process(data : ARRAY (. 10 .) OF INTEGER) : INTEGER;

(* ĐÚNG - Phải dùng BasicType cho tham số *)
FUNCTION Process(index : INTEGER) : INTEGER;
```

---

## Danh sách Lỗi trong Trình biên dịch KPL

Trình biên dịch KPL phát hiện và báo cáo **29 loại lỗi** khác nhau, được chia thành 3 nhóm chính: Lỗi Lexical, Lỗi Syntax, và Lỗi Semantic.

### Định dạng thông báo lỗi

Khi phát hiện lỗi, chương trình sẽ in ra thông báo và **dừng ngay lập tức**:

```
<số_dòng>-<số_cột>:<thông_báo_lỗi>
```

**Ví dụ:**
```
4-3:Undeclared variable.
```

---

### 1. LỖI LEXICAL (Phân tích từ vựng) - 5 lỗi

Các lỗi này được phát hiện bởi **Scanner** khi phân tích mã nguồn thành các token.

| Mã lỗi | Thông báo | Mô tả |
|--------|-----------|-------|
| `ERR_END_OF_COMMENT` | "End of comment expected." | Chú thích không được đóng bằng `*)` |
| `ERR_IDENT_TOO_LONG` | "Identifier too long." | Tên định danh vượt quá độ dài cho phép |
| `ERR_INVALID_CONSTANT_CHAR` | "Invalid char constant." | Ký tự trong dấu nháy đơn không hợp lệ |
| `ERR_INVALID_SYMBOL` | "Invalid symbol." | Ký tự đặc biệt không được định nghĩa trong KPL |
| `ERR_INVALID_IDENT` | "An identifier expected." | Thiếu hoặc sai tên định danh |

**Ví dụ lỗi Lexical:**

```kpl
(* Lỗi: ERR_END_OF_COMMENT *)
PROGRAM Test;
(* Chú thích không có đóng
BEGIN
END.
(* Output: 3-1:End of comment expected. *)

(* Lỗi: ERR_INVALID_CONSTANT_CHAR *)
PROGRAM Test;
VAR c : CHAR;
BEGIN
  c := 'abc';  (* Chỉ được một ký tự *)
END.
(* Output: 4-8:Invalid char constant. *)

(* Lỗi: ERR_INVALID_SYMBOL *)
PROGRAM Test;
BEGIN
  x := 10 @ 5;  (* Ký hiệu @ không hợp lệ *)
END.
(* Output: 3-11:Invalid symbol. *)
```

---

### 2. LỖI SYNTAX (Cú pháp) - 14 lỗi

Các lỗi này được phát hiện bởi **Parser** khi kiểm tra cấu trúc ngữ pháp.

| Mã lỗi | Thông báo | Khi nào xảy ra |
|--------|-----------|----------------|
| `ERR_INVALID_CONSTANT` | "A constant expected." | Thiếu hoặc sai hằng số |
| `ERR_INVALID_TYPE` | "A type expected." | Thiếu hoặc sai kiểu dữ liệu |
| `ERR_INVALID_BASICTYPE` | "A basic type expected." | Thiếu INTEGER hoặc CHAR |
| `ERR_INVALID_VARIABLE` | "A variable expected." | Thiếu hoặc sai biến |
| `ERR_INVALID_FUNCTION` | "A function identifier expected." | Thiếu hoặc sai tên hàm |
| `ERR_INVALID_PROCEDURE` | "A procedure identifier expected." | Thiếu hoặc sai tên thủ tục |
| `ERR_INVALID_PARAMETER` | "A parameter expected." | Tham số không đúng |
| `ERR_INVALID_STATEMENT` | "Invalid statement." | Câu lệnh không đúng cú pháp |
| `ERR_INVALID_COMPARATOR` | "A comparator expected." | Thiếu hoặc sai toán tử so sánh |
| `ERR_INVALID_EXPRESSION` | "Invalid expression." | Biểu thức không đúng |
| `ERR_INVALID_TERM` | "Invalid term." | Term trong biểu thức không đúng |
| `ERR_INVALID_FACTOR` | "Invalid factor." | Factor trong biểu thức không đúng |
| `ERR_INVALID_LVALUE` | "Invalid lvalue in assignment." | Vế trái phép gán không hợp lệ |
| `ERR_INVALID_ARGUMENTS` | "Wrong arguments." | Đối số truyền vào không đúng |

**Ví dụ lỗi Syntax:**

```kpl
(* Lỗi: ERR_INVALID_TYPE *)
PROGRAM Test;
VAR x : String;  (* String không phải kiểu hợp lệ *)
BEGIN
END.
(* Output: 2-9:A type expected. *)

(* Lỗi: ERR_INVALID_STATEMENT *)
PROGRAM Test;
BEGIN
  123;  (* Không phải câu lệnh hợp lệ *)
END.
(* Output: 3-3:Invalid statement. *)

(* Lỗi: ERR_INVALID_LVALUE *)
PROGRAM Test;
VAR x : INTEGER;
BEGIN
  5 := x;  (* Không thể gán cho hằng số *)
END.
(* Output: 4-3:Invalid lvalue in assignment. *)

(* Lỗi: ERR_INVALID_BASICTYPE *)
PROGRAM Test;
TYPE MyArray = ARRAY (. 10 .) OF INTEGER;
FUNCTION Test(arr : MyArray) : INTEGER;  (* Tham số phải là BasicType *)
BEGIN
  Test := 0;
END;
BEGIN
END.
(* Output: 3-22:A basic type expected. *)
```

---

### 3. LỖI SEMANTIC (Ngữ nghĩa) - 10 lỗi

Các lỗi này được phát hiện bởi **Semantic Analyzer** khi kiểm tra ý nghĩa của chương trình.

#### 3.1. Lỗi chưa khai báo (UNDECLARED) - 7 lỗi

| Mã lỗi | Thông báo | Khi nào xảy ra |
|--------|-----------|----------------|
| `ERR_UNDECLARED_IDENT` | "Undeclared identifier." | Sử dụng định danh chưa được khai báo |
| `ERR_UNDECLARED_CONSTANT` | "Undeclared constant." | Sử dụng hằng số chưa được định nghĩa |
| `ERR_UNDECLARED_INT_CONSTANT` | "Undeclared integer constant." | Sử dụng hằng số nguyên chưa khai báo (trong ngữ cảnh cần INT) |
| `ERR_UNDECLARED_TYPE` | "Undeclared type." | Sử dụng kiểu dữ liệu chưa được định nghĩa |
| `ERR_UNDECLARED_VARIABLE` | "Undeclared variable." | Sử dụng biến chưa được khai báo |
| `ERR_UNDECLARED_FUNCTION` | "Undeclared function." | Gọi hàm chưa được định nghĩa |
| `ERR_UNDECLARED_PROCEDURE` | "Undeclared procedure." | Gọi thủ tục chưa được định nghĩa |

**Ví dụ:**

```kpl
(* Lỗi: ERR_UNDECLARED_VARIABLE *)
PROGRAM Test;
BEGIN
  x := 10;  (* Biến x chưa được khai báo *)
END.
(* Output: 3-3:Undeclared variable. *)

(* Lỗi: ERR_UNDECLARED_FUNCTION *)
PROGRAM Test;
VAR result : INTEGER;
BEGIN
  result := Calculate(5);  (* Hàm Calculate chưa được định nghĩa *)
END.
(* Output: 4-13:Undeclared function. *)

(* Lỗi: ERR_UNDECLARED_TYPE *)
PROGRAM Test;
TYPE
  MyArray = ARRAY (. 10 .) OF MyType;  (* MyType chưa được định nghĩa *)
BEGIN
END.
(* Output: 3-31:Undeclared type. *)

(* Lỗi: ERR_UNDECLARED_CONSTANT *)
PROGRAM Test;
CONST
  MAX = MIN + 10;  (* Hằng số MIN chưa được khai báo *)
BEGIN
END.
(* Output: 3-9:Undeclared constant. *)

(* Lỗi: ERR_UNDECLARED_INT_CONSTANT *)
PROGRAM Test;
CONST
  CHAR_A = 'A';
  NUM = +CHAR_A;  (* CHAR_A là CHAR, không phải INT *)
BEGIN
END.
(* Output: 4-10:Undeclared integer constant. *)
```

#### 3.2. Lỗi trùng lặp (DUPLICATE) - 1 lỗi

| Mã lỗi | Thông báo | Khi nào xảy ra |
|--------|-----------|----------------|
| `ERR_DUPLICATE_IDENT` | "Duplicate identifier." | Khai báo trùng tên trong cùng một scope |

**Ví dụ:**

```kpl
(* Lỗi: Trùng biến *)
PROGRAM Test;
VAR x : INTEGER;
VAR x : CHAR;  (* x đã được khai báo ở trên *)
BEGIN
END.
(* Output: 3-5:Duplicate identifier. *)

(* Lỗi: Trùng hằng *)
PROGRAM Test;
CONST
  MAX = 100;
  MAX = 200;  (* MAX đã được định nghĩa *)
BEGIN
END.
(* Output: 4-3:Duplicate identifier. *)

(* Lỗi: Trùng tên hàm *)
PROGRAM Test;

FUNCTION Add(x: INTEGER; y: INTEGER) : INTEGER;
BEGIN
  Add := x + y;
END;

FUNCTION Add(a: INTEGER; b: INTEGER) : INTEGER;  (* Trùng tên *)
BEGIN
  Add := a + b;
END;

BEGIN
END.
(* Output: 8-10:Duplicate identifier. *)

(* OK: Biến cục bộ có thể trùng tên với biến toàn cục - khác scope *)
PROGRAM Test;
VAR x : INTEGER;

PROCEDURE Sub;
VAR x : INTEGER;  (* OK - biến cục bộ che khuất biến toàn cục *)
BEGIN
  x := 10;
END;

BEGIN
  x := 5;
END.
```

#### 3.3. Lỗi không tương thích kiểu (TYPE INCONSISTENCY) - 1 lỗi

| Mã lỗi | Thông báo | Khi nào xảy ra |
|--------|-----------|----------------|
| `ERR_TYPE_INCONSISTENCY` | "Type inconsistency" | Kiểu dữ liệu không khớp |

**Ví dụ:**

```kpl
(* Lỗi: Gán kiểu không khớp *)
PROGRAM Test;
VAR x : INTEGER;
    c : CHAR;
BEGIN
  x := 'A';  (* Gán CHAR cho INTEGER *)
END.
(* Output: 5-8:Type inconsistency *)

(* Lỗi: Phép toán với kiểu không khớp *)
PROGRAM Test;
VAR x : INTEGER;
    c : CHAR;
    result : INTEGER;
BEGIN
  result := x + c;  (* Không thể cộng INTEGER và CHAR *)
END.
(* Output: 6-13:Type inconsistency *)

(* Lỗi: So sánh kiểu không khớp *)
PROGRAM Test;
VAR x : INTEGER;
    c : CHAR;
BEGIN
  IF x = c THEN  (* Không thể so sánh INTEGER và CHAR *)
    x := 0;
END.
(* Output: 5-8:Type inconsistency *)

(* Lỗi: Kiểu trả về không khớp *)
PROGRAM Test;
FUNCTION GetChar : CHAR;
BEGIN
  GetChar := 100;  (* Trả về INTEGER cho hàm kiểu CHAR *)
END;
BEGIN
END.
(* Output: 4-14:Type inconsistency *)

(* Lỗi: Kiểu mảng không khớp *)
PROGRAM Test;
TYPE IntArray = ARRAY (. 10 .) OF INTEGER;
VAR arr1 : IntArray;
    x : INTEGER;
BEGIN
  x := arr1;  (* Không thể gán mảng cho biến đơn *)
END.
(* Output: 6-8:Type inconsistency *)

(* Lỗi: Truy cập mảng trên biến không phải mảng *)
PROGRAM Test;
VAR x : INTEGER;
BEGIN
  x(0) := 10;  (* x không phải mảng *)
END.
(* Output: 4-3:Type inconsistency *)
```

#### 3.4. Lỗi không khớp tham số và đối số - 1 lỗi

| Mã lỗi | Thông báo | Khi nào xảy ra |
|--------|-----------|----------------|
| `ERR_PARAMETERS_ARGUMENTS_INCONSISTENCY` | "The number of arguments and the number of parameters are inconsistent." | Số lượng đối số không bằng số lượng tham số |

**Ví dụ:**

```kpl
(* Lỗi: Thiếu đối số *)
PROGRAM Test;
VAR result : INTEGER;

FUNCTION Add(x: INTEGER; y: INTEGER) : INTEGER;
BEGIN
  Add := x + y;
END;

BEGIN
  result := Add(5);  (* Thiếu đối số thứ 2 *)
END.
(* Output: 10-13:The number of arguments and the number of parameters are inconsistent. *)

(* Lỗi: Thừa đối số *)
PROGRAM Test;

PROCEDURE PrintNumber(n: INTEGER);
BEGIN
  CALL WRITEI(n);
END;

BEGIN
  CALL PrintNumber(5, 10);  (* Thừa đối số *)
END.
(* Output: 8-8:The number of arguments and the number of parameters are inconsistent. *)

(* Lỗi: Không có đối số *)
PROGRAM Test;
VAR result : INTEGER;

FUNCTION Square(x: INTEGER) : INTEGER;
BEGIN
  Square := x * x;
END;

BEGIN
  result := Square;  (* Thiếu dấu ngoặc và đối số *)
END.
(* Output: 10-13:The number of arguments and the number of parameters are inconsistent. *)
```

---

### Các trường hợp đặc biệt về Lvalue

Chỉ các đối tượng sau mới có thể ở vế trái của phép gán (Lvalue):
1. **Biến** (OBJ_VARIABLE)
2. **Tham số** (OBJ_PARAMETER)
3. **Tên hàm** (OBJ_FUNCTION) - nhưng chỉ trong **thân hàm đó**

```kpl
(* Lỗi: Gán cho hằng số *)
PROGRAM Test;
CONST MAX = 100;
BEGIN
  MAX := 200;  (* LỖI: Không thể gán cho hằng số *)
END.
(* Output: 4-3:Invalid lvalue in assignment. *)

(* Lỗi: Gán cho tên hàm bên ngoài hàm *)
PROGRAM Test;
VAR x : INTEGER;

FUNCTION GetValue : INTEGER;
BEGIN
  GetValue := 10;  (* OK - gán cho tên hàm trong thân hàm *)
END;

BEGIN
  GetValue := 20;  (* LỖI: Chỉ được gán trong thân hàm *)
END.
(* Output: 10-3:Invalid lvalue in assignment. *)

(* Lỗi: Gán cho biểu thức *)
PROGRAM Test;
VAR x, y : INTEGER;
BEGIN
  x + y := 10;  (* LỖI: Không thể gán cho biểu thức *)
END.
(* Output: 4-3:Invalid lvalue in assignment. *)
```

---

### Tóm tắt phân loại lỗi

| Loại lỗi | Số lượng | Giai đoạn phát hiện | Thành phần |
|-----------|----------|---------------------|------------|
| **Lexical** | 5 lỗi | Phân tích từ vựng | Scanner |
| **Syntax** | 14 lỗi | Phân tích cú pháp | Parser |
| **Semantic** | 10 lỗi | Phân tích ngữ nghĩa | Semantic Analyzer |
| **Tổng cộng** | **29 lỗi** | | |

### Hành vi khi gặp lỗi

Khi phát hiện bất kỳ lỗi nào, trình biên dịch sẽ:
1. In ra thông báo lỗi với định dạng: `<dòng>-<cột>:<thông_báo>`
2. **Dừng ngay lập tức** (`exit(0)`) - không tiếp tục phân tích

Điều này có nghĩa là trình biên dịch chỉ báo cáo **một lỗi duy nhất** mỗi lần chạy.

---

*Tài liệu này được tạo dựa trên phân tích các file mẫu KPL từ các bài thực hành trình biên dịch và văn phạm BNF của KPL.*
