#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <iup.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

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
    for (int i = 0; i < strlen(source); i++) {
        if (source[i] == ' ')
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
    }
    else {
        char* source = (char*)malloc(sizeof(char) * (source_len + 1));
        char* keyword = (char*)malloc(sizeof(char) * (keyword_len + 1));
        char* res = (char*)malloc(sizeof(char) * (source_len + 1));

        sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
        sprintf(keyword, "%s", IupGetAttribute(text_keyword, "VALUE"));
        sprintf(res, "%s", IupGetAttribute(text_res, "VALUE"));

        sprintf(res, "%s", source);
        replace_char(source, res, keyword);
        encrypt(source, res);

        IupSetAttribute(text_res, "VALUE", res);

        free(source);
        free(keyword);
        free(res);
    }
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
    }
    else {
        char* source = (char*)malloc(sizeof(char) * (source_len + 1));
        char* keyword = (char*)malloc(sizeof(char) * (keyword_len + 1));
        char* res = (char*)malloc(sizeof(char) * (source_len + 1));

        sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
        sprintf(keyword, "%s", IupGetAttribute(text_keyword, "VALUE"));
        sprintf(res, "%s", IupGetAttribute(text_res, "VALUE"));

        sprintf(res, "%s", source);
        replace_char(source, res, keyword);
        descrypt(source, res);

        IupSetAttribute(text_res, "VALUE", res);

        free(source);
        free(keyword);
        free(res);
    }
    return IUP_DEFAULT;
}

int main(int argc, char** argv)
{
    Ihandle* dlg;
    Ihandle* text_source;
    Ihandle* text_keyword;
    Ihandle* text_res;
    Ihandle* ebox;
    Ihandle* btn_encrypt;
    Ihandle* btn_descrypt;
    Ihandle* btn_clear;
    IupOpen(&argc, &argv);

    text_keyword = IupText(NULL);
    text_source = IupText(NULL);
    text_res = IupText(NULL);

    btn_encrypt = IupButton("Encrypt", NULL);
    btn_descrypt = IupButton("Descrypt", NULL);
    btn_clear = IupButton("Clear", NULL);
    ebox = IupVbox(
        text_source,
        text_keyword,
        IupHbox(
            btn_encrypt, btn_descrypt, NULL),
        btn_clear,
        text_res,
        NULL
    );

    IupSetAttribute(ebox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(ebox, "GAP", "10");
    IupSetAttribute(ebox, "MARGIN", "10x10");

    IupSetAttribute(btn_encrypt, "PADDING", "30x2");
    IupSetAttribute(btn_descrypt, "PADDING", "30x2");
    IupSetAttribute(btn_clear, "PADDING", "40x2");

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
    IupSetCallback(btn_clear, "ACTION", (Icallback)btn_clear_cb);

    IupMainLoop();

    IupClose();
    return EXIT_SUCCESS;
}

