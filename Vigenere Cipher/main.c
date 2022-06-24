#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <iup.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <Windows.h>

/*****-Xử lí chuỗi-*****/

int keyword_check(char keyword[]) {
    int n = strlen(keyword);
    for (int i = 0; i < n; i++) {
        if (keyword[i] >= 'A' && keyword[i] <= 'Z') {
            keyword[i] += 32;
        }
        else if (keyword[i] < 'a' || keyword[i] > 'z') {
            return 0;
        }
    }
    return 1;
}

void replace_char(char source[], char res[], char keyword[]) {
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
        if ((source[i] >= 'a' && source[i] <= 'z') || (source[i] >= 'A' && source[i] <= 'Z')) {
            res[i] = keyword[k]; // thay thế từng kí tự chuỗi gốc bằng kí tự trong keyword
            k++;
        }
        else {
            res[i] = source[i];
        }

    }
}

void encrypt(char source[], char res[]) {
    int n = strlen(source);
    for (int i = 0; i < n; i++) {
        if (source[i] == ' ') // nếu là khoảng trắng thì skip
            continue;
        if (source[i] >= 'a' && source[i] <= 'z')
            res[i] = ((source[i] - 97) + (res[i] - 97)) % 26 + 97;
        else if (source[i] >= 'A' && source[i] <= 'Z')
            res[i] = ((source[i] - 65) + (res[i] - 97)) % 26 + 65;
        else
            continue;
    }
}

void descrypt(char source[], char res[]) {
    int n = strlen(source);
    for (int i = 0; i < n; i++) {
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

/*****-Xử lí các nút bấm-*****/

int btn_clear_cb(Ihandle* self) {
    Ihandle* text_res;
    Ihandle* text_source;
    Ihandle* text_keyword;

    text_keyword = IupGetHandle("text_keyword");
    text_source = IupGetHandle("text_source");
    text_res = IupGetHandle("text_res");

    IupSetAttribute(text_source, "VALUE", NULL);
    IupSetAttribute(text_keyword, "VALUE", NULL);
    IupSetAttribute(text_res, "VALUE", NULL);
    return IUP_DEFAULT;
}

int btn_encrypt_cb(Ihandle* self) {
    Ihandle* text_res;
    Ihandle* text_source;
    Ihandle* text_keyword;

    text_keyword = IupGetHandle("text_keyword");
    text_source = IupGetHandle("text_source");
    text_res = IupGetHandle("text_res");

    int source_len = strlen(IupGetAttribute(text_source, "VALUE"));
    int keyword_len = strlen(IupGetAttribute(text_keyword, "VALUE"));

    if (source_len == 0 || keyword_len == 0) {
        IupMessage("Baka do ngoc", "Chua nhap gi kia :v");
        return IUP_DEFAULT;
    }

    char* source = (char*)malloc(sizeof(char) * (source_len + 1));
    char* keyword = (char*)malloc(sizeof(char) * (keyword_len + 1));
    char* res = (char*)malloc(sizeof(char) * (source_len + 1));

    sprintf(keyword, "%s", IupGetAttribute(text_keyword, "VALUE"));

    if (keyword_check(keyword) == 0) {
        IupMessage("Baka do ngoc", "Nhap sai keyword roi kia :v");
        return IUP_DEFAULT;
    }

    sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
    sprintf(res, "%s", IupGetAttribute(text_res, "VALUE"));
    
    sprintf(res, "%s", source);
    replace_char(source, res, keyword);
    encrypt(source, res);

    IupSetAttribute(text_res, "VALUE", res);

    free(source);
    free(keyword);
    free(res);
    return IUP_DEFAULT;
}

int btn_descrypt_cb(Ihandle* self) {
    Ihandle* text_res;
    Ihandle* text_source;
    Ihandle* text_keyword;

    text_keyword = IupGetHandle("text_keyword");
    text_source = IupGetHandle("text_source");
    text_res = IupGetHandle("text_res");

    int source_len = strlen(IupGetAttribute(text_source, "VALUE"));
    int keyword_len = strlen(IupGetAttribute(text_keyword, "VALUE"));

    if (source_len == 0 || keyword_len == 0) {
        IupMessage("Baka do ngoc", "Chua nhap gi kia :v");
        return IUP_DEFAULT;
    }

    char* source = (char*)malloc(sizeof(char) * (source_len + 1));
    char* keyword = (char*)malloc(sizeof(char) * (keyword_len + 1));
    char* res = (char*)malloc(sizeof(char) * (source_len + 1));

    sprintf(keyword, "%s", IupGetAttribute(text_keyword, "VALUE"));

    if (keyword_check(keyword) == 0) {
        IupMessage("Baka do ngoc", "Nhap sai keyword roi kia :v");
        return IUP_DEFAULT;
    }

    sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
    sprintf(res, "%s", IupGetAttribute(text_res, "VALUE"));

    sprintf(res, "%s", source);
    replace_char(source, res, keyword);
    descrypt(source, res);

    IupSetAttribute(text_res, "VALUE", res);

    free(source);
    free(keyword);
    free(res);
    return IUP_DEFAULT;
}

void Vigenere_Cipher() {
    Ihandle* dlg;
    Ihandle* text_source;
    Ihandle* text_keyword;
    Ihandle* text_res;
    Ihandle* element_box;
    Ihandle* btn_encrypt;
    Ihandle* btn_descrypt;
    Ihandle* btn_clear;
    Ihandle* frame_encrypt;
    Ihandle* frame_keyword;
    Ihandle* frame_res;

    text_keyword = IupText(NULL);
    text_source = IupText(NULL);
    text_res = IupText(NULL);

    btn_encrypt = IupButton("Encrypt", NULL);
    btn_descrypt = IupButton("Descrypt", NULL);
    btn_clear = IupButton("Clear", NULL);

    frame_encrypt = IupFrame(text_source);
    frame_keyword = IupFrame(text_keyword);
    frame_res = IupFrame(text_res);

    element_box = IupVbox(
        frame_encrypt,
        frame_keyword,
        IupHbox(
            btn_encrypt,
            btn_descrypt,
            btn_clear,
            NULL),
        frame_res,
        NULL
    );

    IupSetAttribute(frame_encrypt, "TITLE", "Enter plain text/cipher text here:");
    IupSetAttribute(frame_keyword, "TITLE", "Enter keyword here:");
    IupSetAttribute(frame_res, "TITLE", "Result:");

    IupSetAttribute(element_box, "ALIGNMENT", "ACENTER");
    IupSetAttribute(element_box, "GAP", "10");
    IupSetAttribute(element_box, "MARGIN", "10x10");

    IupSetAttribute(btn_encrypt, "PADDING", "30x2");
    IupSetAttribute(btn_descrypt, "PADDING", "30x2");
    IupSetAttribute(btn_clear, "PADDING", "30x2");

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
        element_box
    );

    IupSetAttribute(dlg, "TITLE", "Vigenère Cipher");
    IupSetAttribute(dlg, "SIZE", "QUARTERx200");
    IupSetAttribute(dlg, "RESIZE", "YES");
    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
    IupSetAttribute(dlg, "USERSIZE", NULL);

    IupSetCallback(btn_encrypt, "ACTION", (Icallback)btn_encrypt_cb);
    IupSetCallback(btn_descrypt, "ACTION", (Icallback)btn_descrypt_cb);
    IupSetCallback(btn_clear, "ACTION", (Icallback)btn_clear_cb);
}

int main(int argc, char** argv)
{
    IupOpen(&argc, &argv);

    Vigenere_Cipher();

    IupMainLoop();

    IupClose();
    return EXIT_SUCCESS;
}

