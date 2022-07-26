#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <iup.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include "icon.h"
#include <iupkey.h>

/******************************-Xử lí chuỗi-*********************************/

char* read_file(const char* filename)
{
    int size;
    char* str;
    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        IupMessagef("Error", "Can't open file: %s", filename);
        return NULL;
    }

    /* tính toán độ lớn của file */
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    /* cấp phát bộ nhớ cho chuỗi chứa đường dẫn và thêm kí tự kết thúc */
    str = malloc(size + 1);
    /* đọc dữ liệu của file và truyền vào str */
    fread(str, size, 1, file);
    /* thêm kí tự kết thúc chuỗi */
    str[size] = 0;

    if (ferror(file))
        IupMessagef("Error", "Fail when reading from file: %s", filename);

    fclose(file);
    return str;
}

/*Hàm lưu file*/

void write_file(const char* filename, const char* str, int count)
{
    FILE* file = fopen(filename, "w");
    if (!file)
    {
        IupMessagef("Error", "Can't open file: %s", filename);
        return;
    }

    fwrite(str, 1, count, file);

    if (ferror(file))
        IupMessagef("Error", "Fail when writing to file: %s", filename);

    fclose(file);
}

/*Hàm tạo keyword random*/

void random_keyword(char* keyword, int len) {
    for (int i = 0; i < len; i++) {
        keyword[i] = (rand() % ('z' - 'a' + 1)) + 'a';
    }
    keyword[len] = '\0';
}

/* hàm kiểm tra keyword hợp lệ */
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

/* hàm thay thế chuỗi bằng keyword */
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

/* hàm mã hoá */
void encrypt_vigenere(char source[], char res[]) {
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

/* hàm giải mã */
void descrypt_vigenere(char source[], char res[]) {
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

void descrypt_ceasar(char res[]) {
    int n = strlen(res);
    for (int i = 0; i < n; ++i) {
        if (res[i] >= 'a' && res[i] <= 'z') {
            res[i] = (res[i]  + 3 - 'a') % 26 + 'a';
        }
        else if (res[i] >= 'A' && res[i] <= 'Z') {
            res[i] = (res[i] - 'A' + 3) % 26 + 'A';
        }
    }
}

int is_alphabet(char c) {
    if ('A' <= c && c <= 'Z') return 1;
    if ('a' <= c && c <= 'z') return 1;
    return 0;
}

int encrypt_state_check(char* str, int len, int index) {
    char current = str[index];

    if (index == len - 1 && is_alphabet(current)) {
        return 2;
    }
    if (index == len - 1 && !is_alphabet(current)) {
        return 3;
    }

    if (!is_alphabet(current)) {
        return 3;
    }

    char next = str[index + 1];

    if (is_alphabet(current) && is_alphabet(next)) {
        return 1;
    }
    if (is_alphabet(current) && !is_alphabet(next)) {
        return 2;
    }
    return 0;
}

void encrypt_A1Z26(char* str, char* res) {
    int n = strlen(str);
    int state = 0;
    int offset = 0;
    for (int i = 0; i < n; ++i) {
        state = encrypt_state_check(str, n, i);
        switch (state) {
        case 1:
            offset += sprintf(res + offset, "%d-", tolower(str[i]) - 'a' + 1);
            break;
        case 2:
            offset += sprintf(res + offset, "%d", tolower(str[i]) - 'a' + 1);
            break;
        case 3:
            offset += sprintf(res + offset, "%c", str[i]);
            break;
        }
    }
}

int encrypt_length(char* str, int len) {
    int count = 0;
    int state = 0;
    for (int i = 0; i < len; ++i) {
        state = encrypt_state_check(str, len, i);
        switch (state) {
        case 1:
            if (str[i] >= 'j' && str[i] <= 'z') {
                count += 3;
            }
            else {
                count += 2;
            }
            break;
        case 2:
            if (str[i] >= 'j' && str[i] <= 'z') {
                count += 2;
            }
            else {
                count += 1;
            }
            break;
        case 3:
            count++;
            break;
        }
    }
    return count;
}

int is_number(char c) {
    if ('0' <= c && c <= '9') return 1;
    return 0;
}

int descrypt_state_check(char source[], int len, int index) {
    int current = source[index];

    if (index == 0 && is_number(current)) {
        return 1;
    }
    if (index == 0 && !is_number(current)) {
        return 4;
    }

    int previous = source[index - 1];

    if (index == len - 1 && is_number(current)) {
        return 5;
    }
    if (index == len - 1 && !is_number(current) && is_number(previous)) {
        return 6;
    }
    if (index == len - 1 && !is_number(current) && !is_number(previous)) {
        return 4;
    }

    if (is_number(current) && !is_number(previous)) {
        return 1;
    }
    if (is_number(current) && is_number(previous)) {
        return 2;
    }
    if (!is_number(current) && is_number(previous)) {
        return 3;
    }
    if (!is_number(current) && !is_number(previous)) {
        return 4;
    }
    return 0;
}

void descrypt_A1Z26(char* source, char* res) {
    int n = strlen(source);
    int state;
    int number;
    int offset = 0;
    for (int i = 0; i < n; ++i) {
        state = descrypt_state_check(source, n, i);
        switch (state) {
        case 1:
            number = source[i] - '0';
            break;
        case 2:
            number = number * 10 + source[i] - '0';
            break;
        case 3:
            offset += sprintf(res + offset, "%c", number + 'a' - 1);
            number = 0;
            if (source[i] == '-' && !is_number(source[i + 1])) {
                offset += sprintf(res + offset, "%s", "-");
            }
            if (source[i] != '-')
                offset += sprintf(res + offset, "%c", source[i]);
            break;
        case 4:
            offset += sprintf(res + offset, "%c", source[i]);
            break;
        case 5:
            number = number * 10 + source[i] - '0' + 'a' - 1;
            offset += sprintf(res + offset, "%c", number);
            break;
        case 6:
            offset += sprintf(res + offset, "%c", number + 'a' - 1);
            offset += sprintf(res + offset, "%c", source[i]);
            break;
        }
    }
}

int descrypt_length(char* source, int len) {
    int state = 0;
    int count = 0;
    for (int i = 0; i < len; ++i) {
        state = descrypt_state_check(source, len, i);
        switch (state) {
        case 1:
            count++;
            break;
        case 2:
            break;
        case 3:
            if (source[i] != '-')
                count++;
            else if (source[i] == '-' && !is_number(source[i + 1]))
                count++;
            break;
        case 4:
            count++;
            break;
        case 5:
            if (!is_number(source[i - 1])) {
                count++;
            }
            break;
        case 6:
            count++;
            break;
        }
    }
    return count;
}
/******************************-Xử lí các thành phần trên các submenu-*********************************/

/*nút mở và đọc file */

int item_open_cb(Ihandle* item_open)
{
    Ihandle* multitext = IupGetDialogChild(item_open, "SOURCE");
    Ihandle* filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "OPEN");
    IupSetAttribute(filedlg, "EXTFILTER", "Text Files|*.txt|All Files|*.*|");
    IupSetAttributeHandle(filedlg, "PARENTDIALOG", IupGetDialog(item_open));

    IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

    if (IupGetInt(filedlg, "STATUS") != -1)
    {
        char* filename = IupGetAttribute(filedlg, "VALUE");
        char* str = read_file(filename);
        if (str)
        {
            IupSetStrAttribute(multitext, "VALUE", str);
            free(str);
        }
    }

    IupDestroy(filedlg);
    return IUP_DEFAULT;
}

//nút save file

int item_saveas_cb(Ihandle* item_saveas)
{
    Ihandle* multitext = IupGetDialogChild(item_saveas, "RES");
    Ihandle* filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "SAVE");
    IupSetAttribute(filedlg, "EXTFILTER", "Text Files|*.txt|All Files|*.*|");
    IupSetAttribute(filedlg, "EXTDEFAULT", "txt");
    IupSetAttributeHandle(filedlg, "PARENTDIALOG", IupGetDialog(item_saveas));

    IupPopup(filedlg, IUP_CENTERPARENT, IUP_CENTERPARENT);

    if (IupGetInt(filedlg, "STATUS") != -1)
    {
        char* filename = IupGetAttribute(filedlg, "VALUE");
        char* str = IupGetAttribute(multitext, "VALUE");
        int count = IupGetInt(multitext, "COUNT");
        write_file(filename, str, count);
    }

    IupDestroy(filedlg);
    return IUP_DEFAULT;
}

//nút copy

int item_copy_cb(Ihandle* item_copy)
{
    Ihandle* text_source = IupGetDialogChild(item_copy, "SOURCE");
    Ihandle* text_keyword = IupGetDialogChild(item_copy, "KEYWORD");
    Ihandle* text_res = IupGetDialogChild(item_copy, "RES");
    Ihandle* clipboard = IupClipboard();

    //nếu không chọn vào 1 trong 3 ô text box thì sẽ không copy được

    if (IupGetFocus() == text_source || IupGetFocus() == text_keyword || IupGetFocus() == text_res) { 
        //lấy selected text truyền vào ô nhớ tạm
        IupSetAttribute(clipboard, "TEXT", IupGetAttribute(IupGetFocus(), "SELECTEDTEXT"));
        //sau khi ấn nút sẽ bỏ chọn text
        IupSetAttribute(IupGetFocus(), "SELECTION", "NONE");
    }

    IupDestroy(clipboard);
    return IUP_DEFAULT;
}

int item_cut_cb(Ihandle* item_cut)
{
    Ihandle* text_source = IupGetDialogChild(item_cut, "SOURCE");
    Ihandle* text_keyword = IupGetDialogChild(item_cut, "KEYWORD");
    Ihandle* text_res = IupGetDialogChild(item_cut, "RES");
    Ihandle* clipboard = IupClipboard();

    //kiểm tra coi người dùng có chọn vào 1 trong 3 ô text box để thực hiện

    if (IupGetFocus() == text_source || IupGetFocus() == text_keyword || IupGetFocus() == text_res) {
        //lấy selected text truyền vào ô nhớ tạm
        IupSetAttribute(clipboard, "TEXT", IupGetAttribute(IupGetFocus(), "SELECTEDTEXT"));
        IupSetAttribute(IupGetFocus(), "SELECTEDTEXT", "");
    }

    IupDestroy(clipboard);
    return IUP_DEFAULT;
}

int item_paste_cb(Ihandle* item_paste)
{
    Ihandle* text_source = IupGetDialogChild(item_paste, "SOURCE");
    Ihandle* text_keyword = IupGetDialogChild(item_paste, "KEYWORD");
    Ihandle* text_res = IupGetDialogChild(item_paste, "RES");
    Ihandle* clipboard = IupClipboard();

    //tương tự như trên

    if (IupGetFocus() == text_source || IupGetFocus() == text_keyword || IupGetFocus() == text_res) {
        //chèn vào vị trí đang có của thanh nhập giá trị của ô nhớ tạm
        IupSetAttribute(IupGetFocus(), "INSERT", IupGetAttribute(clipboard, "TEXT"));
    }

    IupDestroy(clipboard);
    return IUP_IGNORE;
}

int item_delete_cb(Ihandle* item_delete) {
    Ihandle* text_source = IupGetDialogChild(item_delete, "SOURCE");
    Ihandle* text_keyword = IupGetDialogChild(item_delete, "KEYWORD");
    Ihandle* text_res = IupGetDialogChild(item_delete, "RES");

    if (IupGetFocus() == text_source || IupGetFocus() == text_keyword || IupGetFocus() == text_res) {
        //cho giá trị rỗng vào selected text
        IupSetAttribute(IupGetFocus(), "SELECTEDTEXT", "");
    }
    return IUP_IGNORE;
}

int item_select_all_cb(Ihandle* item_select_all) {
    Ihandle* text_source = IupGetDialogChild(item_select_all, "SOURCE");
    Ihandle* text_keyword = IupGetDialogChild(item_select_all, "KEYWORD");
    Ihandle* text_res = IupGetDialogChild(item_select_all, "RES");

    if (IupGetFocus() == text_source || IupGetFocus() == text_keyword || IupGetFocus() == text_res) {
        //chọn hết tất cả các text trong ô đang focus nếu hợp lệ
        IupSetAttribute(IupGetFocus(), "SELECTION", "ALL");
    }
    return IUP_DEFAULT;
}

int item_about_cb(Ihandle* item_about) {
    Ihandle* fill, * label1, * label2, * label3, * vbox, * link, * dlg;

    dlg = (Ihandle*)IupGetAttribute(item_about, "ABOUT_DIALOG");

    fill = IupFill();
    label1 = IupLabel("A simple a Vigenère Cipher encoder program written in C\nVersion: beta-0.1.2");
    label2 = IupLabel("Home:");
    label3 = IupLabel("Credit:\nbp82125\nThienAn923");
    link = IupLink("https://github.com/bp82125/Vigenere-Cipher", "github.com/bp82125/Vigenere-Cipher");
    vbox = IupVbox(
        label1,
        IupHbox(label2, link, NULL),
        label3,
        NULL
    );

    IupSetAttribute(label1, "ALIGNMENT", "ALEFT");
    IupSetAttribute(label3, "ALIGNMENT", "ACENTER");

    IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(vbox, "GAP", "5");
    IupSetAttribute(vbox, "MARGIN", "10x10");

    dlg = IupDialog(vbox);
    IupSetAttribute(dlg, "TITLE", "About");
    IupSetAttribute(dlg, "DIALOGFRAME", "Yes");
    IupSetAttributeHandle(dlg, "PARENTDIALOG", IupGetDialog(item_about));

    IupSetAttribute(item_about, "ABOUT_DIALOG", (char*)dlg);
    IupPopup(dlg, IUP_CENTER, IUP_CENTER);
    IupDestroy(dlg);

    return IUP_DEFAULT;
}

int toggle_cb(Ihandle* ih, int state) {
    Ihandle * text_keyword = IupGetDialogChild(ih, "FRAMEKEYWORD");
    Ihandle* toggle = IupGetHandle("toggle_darkmode");
    int toggle_state = IupGetInt(toggle, "VALUE");
    if (toggle_state) {
        IupSetAttribute(text_keyword, "VISIBLE", "NO");
        IupSetAttribute(text_keyword, "FLOATING", "YES");
    }
    else {
        IupSetAttribute(text_keyword, "VISIBLE", "YES");
        IupSetAttribute(text_keyword, "FLOATING", "NO");
    }
    IupRefresh(ih);
    return IUP_DEFAULT;
}

/******************************-Xử lí các submenu-*********************************/

int edit_submenu_cb(Ihandle* ih) {
    Ihandle* clipboard = IupClipboard();

    Ihandle* item_cut = IupGetDialogChild(ih, "CUT");
    Ihandle *item_copy = IupGetDialogChild(ih, "COPY");
    Ihandle *item_paste = IupGetDialogChild(ih, "PASTE");
    Ihandle *item_delete = IupGetDialogChild(ih, "DELETE");
    Ihandle *item_select_all = IupGetDialogChild(ih, "SELECT");

    Ihandle* text_source = IupGetDialogChild(ih, "SOURCE");
    Ihandle* text_keyword = IupGetDialogChild(ih, "KEYWORD");
    Ihandle* text_res = IupGetDialogChild(ih, "RES");

    if (!IupGetInt(clipboard, "TEXTAVAILABLE")) {
        IupSetAttribute(item_paste, "ACTIVE", "NO");
    }
    else {
        IupSetAttribute(item_paste, "ACTIVE", "YES");
    }

    if (IupGetAttribute(text_source, "SELECTEDTEXT")|| IupGetAttribute(text_keyword, "SELECTEDTEXT")|| IupGetAttribute(text_res, "SELECTEDTEXT")) {
        IupSetAttribute(item_cut, "ACTIVE", "YES");
        IupSetAttribute(item_delete, "ACTIVE", "YES");
        IupSetAttribute(item_copy, "ACTIVE", "YES");
    }
    else {
        IupSetAttribute(item_cut, "ACTIVE", "NO");
        IupSetAttribute(item_delete, "ACTIVE", "NO");
        IupSetAttribute(item_copy, "ACTIVE", "NO");
    }

    IupDestroy(clipboard);
    return IUP_DEFAULT;
}

/****************************-Xử lí các nút bấm-*****************************/

/* nút exit */
int item_exit_cb(Ihandle* self) {
    return IUP_CLOSE;
}
/*Nút cancel*/
int btn_cancel_cb(Ihandle* self) {
    return IUP_CLOSE;
}

/* Nút tạo keyword random */

int btn_create_random_keyword_cb(Ihandle* self) {
    Ihandle* text_keyword;
    text_keyword = IupGetHandle("text_keyword");
    
    char* keyword;
    int len = (rand() % (20 - 10 + 1)) + 10;
    keyword = (char*)malloc(sizeof(char) * (len + 1));
    random_keyword(keyword, len);

    IupSetAttribute(text_keyword, "VALUE", keyword);
    free(keyword);
    return IUP_CLOSE;
}
/* nút clear */
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

/* nút encrypt */
int btn_encrypt_vigenere_cb(Ihandle* self) {
    Ihandle* text_res = NULL;
    Ihandle* text_source = NULL;
    Ihandle* text_keyword = NULL;
    Ihandle* toggle = NULL;
    int cypher = 1;

    text_keyword = IupGetHandle("text_keyword");
    text_source = IupGetHandle("text_source");
    text_res = IupGetHandle("text_res");
    toggle = IupGetHandle("toggle");

    int source_len = strlen(IupGetAttribute(text_source, "VALUE"));
    int keyword_len = strlen(IupGetAttribute(text_keyword, "VALUE"));
    int toggle_state = IupGetInt(toggle, "VALUE");

    char* source = (char*)malloc(sizeof(char) * (source_len + 1));
    char* keyword = (char*)malloc(sizeof(char) * (keyword_len + 1));
    char* res = (char*)malloc(sizeof(char) * (source_len + 1));

    if (source_len == 0) {
        IupMessage("Baka do ngoc", "Chua nhap gi kia :v");
        return IUP_DEFAULT;
    }

    switch (cypher) {
    case 1:

        sprintf(keyword, "%s", IupGetAttribute(text_keyword, "VALUE"));

        if (!keyword_check(keyword)) {

            IupMessage("Error!", "Keyword is not suitable");

            return IUP_DEFAULT;

        }

        /*Tạo dialog nếu keyword bỏ trống*/
        if (toggle_state) {
            free(keyword);
            keyword_len = (rand() % (20 - 10 + 1)) + 10;
            keyword = (char*)malloc(sizeof(char) * (keyword_len + 1));
            random_keyword(keyword, keyword_len);
            IupSetAttribute(text_keyword, "VALUE", keyword);
        }
        else if (!toggle_state && keyword_len == 0) {

            Ihandle* button, * button_2, * label, * dlg, * vbox;

            label = IupLabel("You haven't entered any keyword. Do you want to create a random one ?");
            IupSetAttribute(label, "PADDING", "10x20");
            button = IupButton("OK", NULL);
            button_2 = IupButton("Cancel", NULL);
            IupSetAttribute(button, "PADDING", "30x2");
            IupSetAttribute(button_2, "PADDING", "30x2");

            vbox = IupVbox(
                label,
                IupHbox(button, button_2, NULL),
                NULL);
            IupSetAttribute(vbox, "ALIGNMENT", "ARIGHT");
            IupSetAttribute(vbox, "GAP", "10");
            IupSetAttribute(vbox, "MARGIN", "10x10");

            dlg = IupDialog(vbox);
            IupSetAttribute(dlg, "TITLE", "Error!");
            IupSetAttribute(dlg, "MAXBOX", "No");
            IupSetAttribute(dlg, "MINBOX", "No");

            /* Registers callbacks */
            IupSetCallback(button, "ACTION", (Icallback)btn_create_random_keyword_cb);
            IupSetCallback(button_2, "ACTION", (Icallback)btn_cancel_cb);

            IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
            IupMainLoop();
            IupDestroy(dlg); //Hủy bản cảnh báo
            return IUP_DEFAULT;
        }

        sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
        sprintf(res, "%s", IupGetAttribute(text_res, "VALUE"));
        sprintf(res, "%s", source);
        replace_char(source, res, keyword);
        encrypt_vigenere(source, res);

        IupSetAttribute(text_res, "VALUE", res);
        break;
    case 2:
        // Chỗ này là để in số trong khung số.
        // Tạo số mặc định cho khung số
        //Tạo warning nếu không nhập số
        sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
        sprintf(res, "%s", IupGetAttribute(text_res, "VALUE"));
        sprintf(res, "%s", source);
        encrypt_ceasar(source, res);

        IupSetAttribute(text_res, "VALUE", res);
        break;
    };
    free(source);
    free(keyword);
    free(res);
        return IUP_DEFAULT;
 }
 

    /* nút descrypt */
int btn_descrypt_vigenere_cb(Ihandle * self) {
    Ihandle* text_res;
    Ihandle* text_source;
    Ihandle* text_keyword;

    text_keyword = IupGetDialogChild(self, "KEYWORD");
    text_source = IupGetDialogChild(self, "SOURCE");
    text_res = IupGetDialogChild(self, "RES");

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
    descrypt_vigenere(source, res);

    IupSetAttribute(text_res, "VALUE", res);

    free(source);
    free(keyword);
    free(res);
    return IUP_DEFAULT;
 }

    int btn_encrypt_1(Ihandle* self) {
        IupMessage("Test", "1");
        return IUP_DEFAULT;
    }

    int btn_encrypt_ceasar_cb(Ihandle* self) {
        IupMessage("Test", "2");
        return IUP_DEFAULT;
    }

    int btn_descrypt_ceasar_cb(Ihandle* self) {
        Ihandle* text_res;
        Ihandle* text_source;

        text_source = IupGetDialogChild(self, "SOURCE");
        text_res = IupGetDialogChild(self, "RES");

        int source_len = strlen(IupGetAttribute(text_source, "VALUE"));

        if (source_len == 0) {
            IupMessage("Baka do ngoc", "Chua nhap gi kia :v");
            return IUP_DEFAULT;
        }

        char* source = (char*)malloc(sizeof(char) * (source_len + 1));
        char* res = (char*)malloc(sizeof(char) * (source_len + 1));

        sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
        sprintf(res, "%s", IupGetAttribute(text_res, "VALUE"));

        sprintf(res, "%s", source);
        descrypt_ceasar(res);

        IupSetAttribute(text_res, "VALUE", res);

        free(source);
        free(res);
        return IUP_DEFAULT;
    }

    int btn_descrypt_A1Z26_cb(Ihandle* self) {
        Ihandle* text_res;
        Ihandle* text_source;

        text_source = IupGetDialogChild(self, "SOURCE");
        text_res = IupGetDialogChild(self, "RES");

        int source_len = strlen(IupGetAttribute(text_source, "VALUE"));
        int res_len = descrypt_length(IupGetAttribute(text_source, "VALUE"), source_len);

        if (source_len == 0) {
            IupMessage("Baka do ngoc", "Chua nhap gi kia :v");
            return IUP_DEFAULT;
        }

        char* source = (char*)malloc(sizeof(char) * (source_len + 1));
        char* res = (char*)malloc(sizeof(char) * (res_len + 1));

        sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
        sprintf(res, "%s", "");

        descrypt_A1Z26(source,res);

        IupSetAttribute(text_res, "VALUE", res);

        free(source);
        free(res);
        return IUP_DEFAULT;
    }

    int btn_encrypt_A1Z26_cb(Ihandle* self) {
        Ihandle* text_res;
        Ihandle* text_source;

        text_source = IupGetDialogChild(self, "SOURCE");
        text_res = IupGetDialogChild(self, "RES");

        int source_len = strlen(IupGetAttribute(text_source, "VALUE"));
        int res_len = encrypt_length(IupGetAttribute(text_source, "VALUE"),source_len);

        if (source_len == 0) {
            IupMessage("Baka do ngoc", "Chua nhap gi kia :v");
            return IUP_DEFAULT;
        }

        char* source = (char*)malloc(sizeof(char) * (source_len + 1));
        char* res = (char*)malloc(sizeof(char) * (res_len + 1));

        sprintf(source, "%s", IupGetAttribute(text_source, "VALUE"));
        sprintf(res, "%s","");

        encrypt_A1Z26(source, res);

        IupSetAttribute(text_res, "VALUE", res);

        free(source);
        free(res);
        return IUP_DEFAULT;
    }

    int btn_encrypt_4(Ihandle* self) {
        IupMessage("Test", "4");
        return IUP_DEFAULT;
    }

    int btn_encrypt_5(Ihandle* self) {
        IupMessage("Test", "5");
        return IUP_DEFAULT;
    }

    int btn_encrypt_6(Ihandle* self) {
        IupMessage("Test", "6");
        return IUP_DEFAULT;
    }

    int list_cb(Ihandle* self, char* t, int i, int v)
    {
        Ihandle* btn_encrypt = IupGetDialogChild(self, "ENCRYPT");
        Ihandle* btn_descrypt = IupGetDialogChild(self, "DESCRYPT");
        Ihandle* frame_keyword = IupGetDialogChild(self, "FRAMEKEYWORD");
        switch (i) {
        case 1:
            IupSetCallback(btn_encrypt, "ACTION", (Icallback)btn_encrypt_1);
            IupSetAttribute(frame_keyword, "VISIBLE", "YES");
            IupSetAttribute(frame_keyword, "FLOATING", "NO");
            IupRefresh(self);
            break;
        case 2:
            IupSetCallback(btn_encrypt, "ACTION", (Icallback)btn_encrypt_ceasar_cb);
            IupSetCallback(btn_descrypt, "ACTION", (Icallback)btn_descrypt_ceasar_cb);

            IupSetAttribute(frame_keyword, "VISIBLE", "NO");
            IupSetAttribute(frame_keyword, "FLOATING", "YES");
            IupRefresh(self);
            break;
        case 3:
            IupSetCallback(btn_encrypt, "ACTION", (Icallback)btn_encrypt_A1Z26_cb);
            IupSetCallback(btn_descrypt, "ACTION", (Icallback)btn_descrypt_A1Z26_cb);

            IupSetAttribute(frame_keyword, "VISIBLE", "NO");
            IupSetAttribute(frame_keyword, "FLOATING", "YES");
            IupRefresh(self);
            break;
        case 4:
            IupSetCallback(btn_encrypt, "ACTION", (Icallback)btn_encrypt_4);
            IupSetAttribute(frame_keyword, "VISIBLE", "NO");
            IupSetAttribute(frame_keyword, "FLOATING", "YES");
            IupRefresh(self);
            break;
        case 5:
            IupSetCallback(btn_encrypt, "ACTION", (Icallback)btn_encrypt_5);
            IupSetAttribute(frame_keyword, "VISIBLE", "NO");
            IupSetAttribute(frame_keyword, "FLOATING", "YES");
            IupRefresh(self);
            break;
        case 6:
            IupSetCallback(btn_encrypt, "ACTION", (Icallback)btn_encrypt_6);
            IupSetAttribute(frame_keyword, "VISIBLE", "NO");
            IupSetAttribute(frame_keyword, "FLOATING", "YES");
            IupRefresh(self);
            break;
        }
        return IUP_DEFAULT;
    }

    /*********************-Hàm chính trong chương trình-***********************/

    int main(int argc, char** argv)
    {
        srand(time(NULL));

        Ihandle* dlg, * element_box, * vbox;
        Ihandle* text_source, * text_keyword, * text_res;
        Ihandle* btn_encrypt, * btn_descrypt, * btn_clear;
        Ihandle* frame_encrypt, * frame_keyword, * frame_res;
        Ihandle* item_open, * item_saveas, * item_exit, * item_about;
        Ihandle* item_copy, * item_paste, * item_cut, * item_delete, * item_select_all;
        Ihandle* file_menu, *edit_menu, * help_menu, * main_menu;
        Ihandle* sub_file_menu, * sub_about_menu, * sub_edit_menu;
        Ihandle* toggle;
        Ihandle* toolbar_hb, * btn_open, * btn_save, * toggle_darkmode, * fill, * btn_cut, * btn_copy, *btn_paste;
        Ihandle* list;

        IupOpen(&argc, &argv);

        create_images_icon();

        toggle = IupToggle("Random", NULL);
        IupSetHandle("toggle", toggle);

        //khai báo text box

        text_keyword = IupText(NULL);
        text_source = IupText(NULL);
        text_res = IupText(NULL);

        IupSetAttribute(text_source, "NAME", "SOURCE");
        IupSetAttribute(text_keyword, "NAME", "KEYWORD");
        IupSetAttribute(text_res, "NAME", "RES");

        //khai báo các nút

        btn_encrypt = IupButton("Encrypt", NULL);
        btn_descrypt = IupButton("Descrypt", NULL);
        btn_clear = IupButton("Clear", NULL);

        IupSetAttribute(btn_encrypt, "NAME", "ENCRYPT");
        IupSetAttribute(btn_descrypt, "NAME", "DESCRYPT");
        IupSetAttribute(btn_clear, "NAME", "CLEAR");

        //khai báo và điều chỉnh các nút trên toolbar
        fill = IupFill();
        IupSetAttribute(fill, "SIZE", "4");

        btn_open = IupButton(NULL, NULL);
        btn_save = IupButton(NULL, NULL);
        btn_cut = IupButton(NULL, NULL);
        btn_copy = IupButton(NULL, NULL);
        btn_paste = IupButton(NULL, NULL);
        toggle_darkmode = IupToggle(NULL, NULL);

        IupSetAttribute(btn_open, "IMAGE", "OpenFolder");
        IupSetAttribute(btn_open, "RASTERSIZE", "30x30");
        IupSetAttribute(btn_open, "FLAT", "Yes");
        IupSetAttribute(btn_open, "CANFOCUS", "No");

        IupSetAttribute(btn_save, "IMAGE", "SaveAs");
        IupSetAttribute(btn_save, "FLAT", "Yes");
        IupSetAttribute(btn_save, "RASTERSIZE", "30x30");
        IupSetAttribute(btn_save, "CANFOCUS", "No");
        
        IupSetAttribute(btn_cut, "IMAGE", "Cut");
        IupSetAttribute(btn_cut, "FLAT", "Yes");
        IupSetAttribute(btn_cut, "RASTERSIZE", "30x30");
        IupSetAttribute(btn_cut, "CANFOCUS", "No");

        IupSetAttribute(btn_copy, "IMAGE", "Copy");
        IupSetAttribute(btn_copy, "FLAT", "Yes");
        IupSetAttribute(btn_copy, "RASTERSIZE", "30x30");
        IupSetAttribute(btn_copy, "CANFOCUS", "No");

        IupSetAttribute(btn_paste, "IMAGE", "Paste");
        IupSetAttribute(btn_paste, "FLAT", "Yes");
        IupSetAttribute(btn_paste, "RASTERSIZE", "30x30");
        IupSetAttribute(btn_paste, "CANFOCUS", "No");

        IupSetAttribute(toggle_darkmode, "IMAGE", "DarkTheme");
        IupSetAttribute(toggle_darkmode, "FLAT", "Yes");
        IupSetAttribute(toggle_darkmode, "CANFOCUS", "Yes");

        // khai báo các khung

        frame_encrypt = IupFrame(text_source);
        frame_keyword = IupFrame(IupHbox(text_keyword, toggle, NULL));
        frame_res = IupFrame(text_res);

        //khai báo các phần tử sub menu file

        item_open = IupItem("Open\tCtrl + O", NULL);
        item_saveas = IupItem("Save as\tCtrl + S", NULL);
        item_exit = IupItem("Exit\tAlt + F4", NULL);

        IupSetAttribute(item_open, "TITLEIMAGE", "OpenFolder");
        IupSetAttribute(item_saveas, "TITLEIMAGE", "SaveAs");

        //khai báo các phần tử sub menu edit

        item_cut = IupItem("Cut\tCtrl + X",NULL);
        item_copy = IupItem("Copy\tCtrl + C",NULL);
        item_paste = IupItem("Paste\tCtrl + V",NULL);
        item_delete = IupItem("Delete\tDel",NULL);
        item_select_all = IupItem("Select All\tCtrl + A", NULL);

        IupSetAttribute(item_cut, "NAME", "CUT");
        IupSetAttribute(item_copy, "NAME", "COPY");
        IupSetAttribute(item_paste, "NAME", "PASTE");
        IupSetAttribute(item_delete, "NAME", "DELETE");
        IupSetAttribute(item_select_all, "NAME", "SELECT");

        IupSetAttribute(item_cut, "TITLEIMAGE", "Cut");
        IupSetAttribute(item_copy, "TITLEIMAGE", "Copy");
        IupSetAttribute(item_paste, "TITLEIMAGE", "Paste");
        IupSetAttribute(item_delete, "TITLEIMAGE", "Cancel");
        IupSetAttribute(item_select_all, "TITLEIMAGE", "Select");

        //khai báo list đổi mã

        list = IupList(NULL);
        IupSetAttributes(list, "1=\"Vigenere Cipher\", 2=\"Ceasar Cipher\", 3=\"A1Z26 Cipher\", 4=\"Binary\", 5=\"Decimal\",6=\"Hex\","
            "DROPDOWN=YES , VALUE=\"+--+--\", SIZE=EIGHTHxEIGHTH");

        IupSetCallback(list, "ACTION", (Icallback)list_cb);

        //khai báo các phần tử sub menu help

        item_about = IupItem("About", NULL);


        //khai báo các thành phần của sub menu file

        file_menu = IupMenu(
            item_open,
            item_saveas,
            IupSeparator(),
            item_exit,
            NULL);

        //khai báo phần tử của sub menu edit

        edit_menu = IupMenu(
            item_cut,
            item_copy,
            item_paste,
            item_delete,
            IupSeparator(),
            item_select_all,
            NULL
        );

        //khai báo các thành phần của sub menu help

        help_menu = IupMenu(
            item_about,
            NULL);

        sub_file_menu = IupSubmenu("File", file_menu);
        sub_about_menu = IupSubmenu("Help", help_menu);
        sub_edit_menu = IupSubmenu("Edit", edit_menu);

        IupSetCallback(edit_menu, "OPEN_CB", edit_submenu_cb);

        //khai báo menu chính

        main_menu = IupMenu(sub_file_menu,sub_edit_menu,sub_about_menu, NULL);

        //cho các thành phần vào toolbar

        toolbar_hb = IupHbox(
            fill,
            btn_open,
            btn_save,
            IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
            btn_cut,
            btn_copy,
            btn_paste,
            IupFill(),
            list,
            toggle_darkmode,
            NULL
        );

        //cho các thành phần vào hộp các phần tử

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

        vbox = IupVbox(
            toolbar_hb,
            element_box,
            NULL
        );
        //thêm tựa đề cho các khung

        IupSetAttribute(frame_encrypt, "TITLE", "Enter plain text/cipher text here:");
        IupSetAttribute(frame_keyword, "TITLE", "Enter keyword here:");
        IupSetAttribute(frame_res, "TITLE", "Result:");

        IupSetAttribute(frame_keyword, "NAME", "FRAMEKEYWORD");

        //điều chỉnh hộp phần tử

        IupSetAttribute(toolbar_hb, "ALIGNMENT", "ALEFT");
        IupSetAttribute(toolbar_hb, "MARGIN", "5x5");
        IupSetAttribute(toolbar_hb, "GAP", "1");

        IupSetAttribute(element_box, "ALIGNMENT", "ACENTER");
        IupSetAttribute(element_box, "GAP", "10");
        IupSetAttribute(element_box, "MARGIN", "10x10");

        //điều chỉnh các nút

        IupSetAttribute(btn_encrypt, "PADDING", "30x2");
        IupSetAttribute(btn_descrypt, "PADDING", "30x2");
        IupSetAttribute(btn_clear, "PADDING", "30x2");

        //điều chỉnh các text box

        IupSetAttribute(text_source, "MULTILINE", "YES");
        IupSetAttribute(text_source, "EXPAND", "YES");
        IupSetAttribute(text_source, "VISIBLELINES", "5");

        IupSetAttribute(text_res, "MULTILINE", "YES");
        IupSetAttribute(text_res, "EXPAND", "YES");
        IupSetAttribute(text_res, "VISIBLELINES", "3");

        IupSetAttribute(text_keyword, "MULTILINE", "NO");
        IupSetAttribute(text_keyword, "EXPAND", "HORIZONTAL");

        //tạo các handle để các text box có thể xài global

        IupSetHandle("text_source", text_source);
        IupSetHandle("text_res", text_res);
        IupSetHandle("text_keyword", text_keyword);
        IupSetHandle("toggle_darkmode", toggle_darkmode);

        // thêm hộp phần tử vào dialog

        dlg = IupDialog(
            vbox
        );

        IupSetHandle("dlg", dlg);

        //điều chỉnh dialog
        IupSetAttribute(dlg, "TITLE", "Vigenère Cipher");
        IupSetAttributeHandle(dlg, "MENU", main_menu);
        IupSetAttribute(dlg, "SIZE", "THIRDx250");
        IupSetAttribute(dlg, "RESIZE", "YES");
        IupShowXY(dlg, IUP_CENTER, IUP_CENTER);

        //gán các event cho các nút

        IupSetCallback(btn_encrypt, "ACTION", (Icallback)btn_encrypt_vigenere_cb);
        IupSetCallback(btn_descrypt, "ACTION", (Icallback)btn_descrypt_vigenere_cb);
        IupSetCallback(btn_clear, "ACTION", (Icallback)btn_clear_cb);

        //gán event cho các nút trong menu

        IupSetCallback(item_open, "ACTION", (Icallback)item_open_cb);
        IupSetCallback(item_exit, "ACTION", (Icallback)item_exit_cb);
        IupSetCallback(item_about, "ACTION", (Icallback)item_about_cb);
        IupSetCallback(item_saveas, "ACTION", (Icallback)item_saveas_cb);

        IupSetCallback(item_copy, "ACTION", (Icallback)item_copy_cb);
        IupSetCallback(item_cut, "ACTION", (Icallback)item_cut_cb);
        IupSetCallback(item_paste, "ACTION", (Icallback)item_paste_cb);
        IupSetCallback(item_delete, "ACTION", (Icallback)item_delete_cb);
        IupSetCallback(item_select_all, "ACTION", (Icallback)item_select_all_cb);

        //gán event cho các nút trong toolbar

        IupSetCallback(btn_open, "ACTION", (Icallback)item_open_cb);
        IupSetCallback(btn_save, "ACTION", (Icallback)item_saveas_cb);
        IupSetCallback(btn_copy, "ACTION", (Icallback)item_copy_cb);
        IupSetCallback(btn_cut, "ACTION", (Icallback)item_cut_cb);
        IupSetCallback(btn_paste, "ACTION", (Icallback)item_paste_cb);
        IupSetCallback(toggle_darkmode, "ACTION", (Icallback)toggle_cb);

        //định nghĩa các shortcut

        IupSetCallback(dlg, "K_cO", (Icallback)item_open_cb);
        IupSetCallback(dlg, "K_cS", (Icallback)item_saveas_cb);
        IupSetCallback(dlg, "K_cC", (Icallback)item_copy_cb);
        IupSetCallback(dlg, "K_cX", (Icallback)item_cut_cb);
        IupSetCallback(dlg, "K_cV", (Icallback)item_paste_cb);
        IupSetCallback(dlg, "K_DEL", (Icallback)item_delete_cb);
        IupSetCallback(dlg, "K_cA", (Icallback)item_select_all_cb);
        IupSetCallback(dlg, "K_mF4", (Icallback)item_exit_cb);
        
        IupMainLoop();

        IupClose();
        return EXIT_SUCCESS;
}