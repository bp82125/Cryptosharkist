# Encrypting Strings with Vigenère Cipher

## Requirements 
[Microsoft Visual C++ 2015-2022 Redistribute x86](https://docs.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)

## Features
- Can encrypt strings with lowercase, uppercase, and special characters <br />
- Unlimited string length for encryption <br />
- User-friendly graphical interface, using [IUP](https://www.tecgraf.puc-rio.br/iup/) <br />
- Able to initialize random keywords and generate random keywords with multiple consecutive inputs <br />
- Read and encrypt .txt files, saving output in .txt format as well <br />

## Future Goals
- Add icons to functional buttons, integrate a logo into the software <br />
- Implement a dark mode <br />
- Introduce a list to allow encryption of various cipher types <br />

## Encryption Process
Suppose we have a given string:

  ```
  The quick brown fox jumps over the lazy dog
  ```
  Along with a provided keyword, for example:
  ```
  huy
  ```
  First, we replace each character of the original string with characters from the keyword:
  ```
  huy huyhu yhuyh uyh huyhu yhuy huy huyh uyh
  ```
  We use the following table to find the encryption character: <br >
  
  ![Vigenere Table](https://crypto.interactive-maths.com/uploads/1/1/3/4/11345755/1889186_orig.jpg)
  
  The character in the original string corresponds to the column position, and the character from the replacement string corresponds to the row position <br >
  For example, for the first 3 characters of each string, the corresponding encryption result would be:
  ```
  [T][h] = A
  [h][u] = b
  [e][y] = c
  ```
  Similarly, for the remaining characters, we get the encrypted string as follows:
  ```
  Abc xogje zyiuu zme dstjq vpcy nfl fygs bva
  ```
 
