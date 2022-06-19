#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <iup.h>
#include <stdio.h>
#include <string.h>

#define M 26
#define N 26

unsigned char Map1[M][N];  // mảng Vigenere với chữ hoa
unsigned char Map2[M][N];  // mảng Vigenere với chữ thường

void Init_Vigenere_Map1() {   // khởi tạo bảng chữ hoa
    for (int i = 0; i < M; ++i) {
        Map1[i][0] = 'A' + i;   // đầu mỗi hàng là chữ cái theo thứ tự A-Z
        for (int j = 1; j < N; ++j) {
            Map1[i][j] = Map1[i][0] + j;  // thêm từ kí tự vào các ô trong từng hàng
            if (Map1[i][j] > 'Z') {
                Map1[i][j] -= 26;  // nếu vượt quá Z thì trở lại vị trí ban đầu là A
            }
        }
    }
}

void Init_Vigenere_Map2() { // khởi tạo bảng chữ thường
    for (int i = 0; i < M; ++i) {
        Map2[i][0] = 'a' + i;
        for (int j = 1; j < N; ++j) {     // giống trên
            Map2[i][j] = Map2[i][0] + j;
            if (Map2[i][j] > 'z') {
                Map2[i][j] -= 26;
            }
        }
    }
}

void replace_char(char res[], char keyword[]) {
    int k = 0; //biến đếm kí tự của chuỗi keyword
    int n = strlen(res);
    int m = strlen(keyword);
    for (int i = 0; i < n; ++i) {
        if (res[i] == ' ') { // nếu gặp khoảng trống thì đi tiếp
            continue;
        }
        if (k >= m) {
            k = 0; //nếu vượt quá phạm vi keyword thì biến đếm trở về vị trí đầu
        }
        res[i] = keyword[k]; // thay thế từng kí tự chuỗi gốc bằng kí tự trong keyword
        k++;
    }
}

void encrypt(char source[], char res[]) {
    for (int i = 0; i < strlen(source); i++) {
        if (source[i] == ' ') {
            continue;
        }
        if (source[i] >= 'a' && source[i] <= 'z') {
            res[i] = Map2[source[i] - 'a'][res[i] - 'a'];
        }
        if (source[i] >= 'A' && source[i] <= 'Z') {
            res[i] = Map1[source[i] - 'A'][res[i] - 'a'];
        }
    }
}

void descrypt(char source[], char res[]) {
    for (int i = 0; i < strlen(source); i++) {
        if (source[i] == ' ') {
            continue;
        }
        if (source[i] >= 'a' && source[i] <= 'z')
            if ((source[i] - (res[i] - 'a')) >= 'a')
                res[i] = source[i] - (res[i] - 'a');
            else if ((source[i] - (res[i] - 'a')) < 'a')
                res[i] = source[i] - (res[i] - 'a') + 26;

        if (source[i] >= 'A' && source[i] <= 'Z')
            if ((source[i] - ((res[i] - 32) - 'A')) >= 'A')
                res[i] = source[i] - ((res[i] - 32) - 'A');
            else if ((source[i] - ((res[i] - 32) - 'A')) < 'A')
                res[i] = source[i] - ((res[i] - 32) - 'A') + 26;
    }
}

int btn_encrypt_cb(Ihandle* self) {
    Ihandle* text_res;
    Ihandle* text_source;
    Ihandle* text_keyword;

    char source[256];
    char keyword[256];
    char res[256];

    text_keyword = IupGetHandle("text_keyword");
    text_source = IupGetHandle("text_source");
    text_res = IupGetHandle("text_res");

    sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
    sprintf(keyword, "%s", IupGetAttribute(text_keyword, "VALUE"));
    sprintf(res, "%s", IupGetAttribute(text_res, "VALUE"));

    sprintf(res, "%s", source);
    replace_char(res, keyword);
    encrypt(source, res);

    IupSetAttribute(text_res, "VALUE", res);
    return IUP_DEFAULT;
}

int btn_descrypt_cb(Ihandle* self) {
    Ihandle* text_res;
    Ihandle* text_source;
    Ihandle* text_keyword;

    char source[256];
    char keyword[256];
    char res[256];

    text_keyword = IupGetHandle("text_keyword");
    text_source = IupGetHandle("text_source");
    text_res = IupGetHandle("text_res");

    sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
    sprintf(keyword, "%s", IupGetAttribute(text_keyword, "VALUE"));
    sprintf(res, "%s", IupGetAttribute(text_res, "VALUE"));

    sprintf(res, "%s", source);
    replace_char(res, keyword);
    descrypt(source, res);

    IupSetAttribute(text_res, "VALUE", res);
    return IUP_DEFAULT;
}

int main(int argc, char** argv)
{
    Init_Vigenere_Map1();
    Init_Vigenere_Map2();

    Ihandle* dlg;
    Ihandle* text_source;
    Ihandle* text_keyword;
    Ihandle* text_res;
    Ihandle* ebox;
    Ihandle* btn_encrypt;
    Ihandle* btn_descrypt;
    IupOpen(&argc, &argv);

    text_keyword = IupText(NULL);
    text_source = IupText(NULL);
    text_res = IupText(NULL);

    btn_encrypt = IupButton("Encrypt", NULL);
    btn_descrypt = IupButton("Descrypt", NULL);
    ebox = IupVbox(
        text_source,
        text_keyword,
        IupHbox(
            btn_encrypt, btn_descrypt, NULL),
        text_res,
        NULL
    );

    IupSetAttribute(ebox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(ebox, "GAP", "10");
    IupSetAttribute(ebox, "MARGIN", "10x10");

    IupSetAttribute(btn_encrypt, "PADDING", "30x2");
    IupSetAttribute(btn_descrypt, "PADDING", "30x2");

    IupSetAttribute(text_source, "MULTILINE", "YES");
    IupSetAttribute(text_source, "EXPAND", "YES");
    IupSetAttribute(text_source, "VISIBLELINES", "5");

    IupSetAttribute(text_res, "MULTILINE", "YES");
    IupSetAttribute(text_res, "EXPAND", "YES");
    IupSetAttribute(text_res, "VISIBLELINES", "3");

    IupSetAttribute(text_keyword, "MULTILINE", "NO");
    IupSetAttribute(text_keyword, "EXPAND", "HORIZONTAL");


    IupSetHandle("text_source", text_source);
    IupSetHandle("text_res", text_res);
    IupSetHandle("text_keyword", text_keyword);

    dlg = IupDialog(
        ebox
    );

    IupSetAttribute(dlg, "TITLE", "Vigenère Cipher");
    IupSetAttribute(dlg, "SIZE", "QUARTERx200");
    IupSetAttribute(dlg, "RESIZE", "YES");
    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
    IupSetAttribute(dlg, "USERSIZE", NULL);

    IupSetCallback(btn_encrypt, "ACTION", (Icallback)btn_encrypt_cb);
    IupSetCallback(btn_descrypt, "ACTION", (Icallback)btn_descrypt_cb);

    IupMainLoop();

    IupClose();
    return EXIT_SUCCESS;
}

