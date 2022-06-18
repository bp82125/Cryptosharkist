# Mã hoá chuỗi bằng Vigenère cipher
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
## Mục tiêu
- Có thể mã hoá chuỗi với cả kí tự thường, hoa và các kí tự đặc biệt khác <br />
- Mã hoá được nhiều chuỗi liên tiếp <br />
- Có thể khởi tạo keyword ngẫu nhiên <br />
- Tạo giao diện đồ hoạ tương tác <br />

 
