# Mã hoá chuỗi bằng Vigenère cipher
## Yêu cầu 
[Microsoft Visual C++ 2015-2022 Redistribute x86](https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)
## Tính năng
- Có thể mã hoá chuỗi với cả kí tự thường, hoa và các kí tự đặc biệt khác <br />
- Mã hoá không giới hạn số lượng kí tự của chuỗi <br />
- Giao diện đồ hoạ thân thiện với người dùng, sử dụng [IUP](https://www.tecgraf.puc-rio.br/iup/) <br />
## Mục tiêu tương lai
- Có thể khởi tạo keyword ngẫu nhiên <br />
- Thêm nút clear có thể xoá hết tất cả kí tự <br />
- Đọc và mã hoá file .txt, lưu output <br />
- Thêm dark mode <br />
## Cách mã hoá
  Giả sử với một chuỗi cho trước sau:
  ```
  The quick brown fox jumps over the lazy dog
  ```
  Kèm theo đó là một keyword cho trước, ví dụ:
  ```
  huy
  ```
  Đầu tiên chúng ta sẽ thay thế từng kí tự của chuỗi gốc bằng các kí tự của keyword:
  ```
  huy huyhu yhuyh uyh huyhu yhuy huy huyh uyh
  ```
  Sử dụng bảng sau để tra kí tự mã hoá: <br >
  
  ![Bang Vigenere](https://crypto.interactive-maths.com/uploads/1/1/3/4/11345755/1889186_orig.jpg)
  
  Kí tự ở chuỗi gốc tượng trưng cho vị trí cột, kí tự của chuỗi thay thế tượng trưng cho hàm ngang <br >
  Ví dụ, với 3 kí tự đầu tiên của mỗi chuỗi, kết quả mã hoá tương ứng sẽ là:
  ```
  [T][h] = A
  [h][u] = b
  [e][y] = c
  ```
  Tương tự với các kí tự còn lại, ta được chuỗi đã mã hoá sau:
  ```
  Abc xogje zyiuu zme dstjq vpcy nfl fygs bva
  ```
 
