#define _CRT_SECURE_NO_WARNINGS

// ntr is my morning routine
#include <stdlib.h>
#include <iup.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

// test vigenere
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

/* hàm giải mã */
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

/****************************-Xử lí các nút bấm-*****************************/

/*nút mở và đọc file */
int btn_open_cb(void)
{
    Ihandle* text_source = IupGetHandle("text_source");
    Ihandle* filedlg = IupFileDlg();
    IupSetAttribute(filedlg, "DIALOGTYPE", "OPEN");
    IupSetAttribute(filedlg, "EXTFILTER", "Text Files|*.txt|All Files|*.*|");

    IupPopup(filedlg, IUP_CENTER, IUP_CENTER);

    if (IupGetInt(filedlg, "STATUS") != -1)
    {
        char* filename = IupGetAttribute(filedlg, "VALUE");
        char* str = read_file(filename);
        if (str)
        {
            IupSetStrAttribute(text_source, "VALUE", str);
            free(str);
        }
    }

    IupDestroy(filedlg);
    return IUP_DEFAULT;
}

/* nút exit */
int btn_exit_cb(Ihandle* self) {
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
int btn_encrypt_cb(Ihandle* self) {
    Ihandle* text_res;
    Ihandle* text_source;
    Ihandle* text_keyword;
    Ihandle* toggle;

    text_keyword = IupGetHandle("text_keyword");
    text_source = IupGetHandle("text_source");
    text_res = IupGetHandle("text_res");
    toggle = IupGetHandle("toggle");

    int source_len = strlen(IupGetAttribute(text_source, "VALUE"));
    int keyword_len = strlen(IupGetAttribute(text_keyword, "VALUE"));
    int toggle_state = IupGetInt(toggle, "VALUE");
    if (source_len == 0) {
        IupMessage("Baka do ngoc", "Chua nhap gi kia :v");
        return IUP_DEFAULT;
    }

    char* source = (char*)malloc(sizeof(char) * (source_len + 1));
    char* keyword = (char*)malloc(sizeof(char) * (keyword_len + 1));
    char* res = (char*)malloc(sizeof(char) * (source_len + 1));

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
    else if (!toggle_state && keyword_len ==0) {
       
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
        encrypt(source, res);

        IupSetAttribute(text_res, "VALUE", res);

        free(source);
        free(keyword);
        free(res);
        return IUP_DEFAULT;
 }
 

    /* nút descrypt */
    int btn_descrypt_cb(Ihandle * self) {
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

    /*********************-Hàm chính trong chương trình-***********************/

    int main(int argc, char** argv)
    {
        srand(time(NULL));

        Ihandle* dlg, * element_box;
        Ihandle* text_source, * text_keyword, * text_res;
        Ihandle* btn_encrypt, * btn_descrypt, * btn_clear;
        Ihandle* frame_encrypt, * frame_keyword, * frame_res;
        Ihandle* item_open, * item_saveas, * item_exit;
        Ihandle* file_menu, * sub1_menu, * main_menu;
        Ihandle* toggle;

        IupOpen(&argc, &argv);

        toggle = IupToggle("Random", NULL);
        IupSetHandle("toggle", toggle);

        //khai báo text box

        text_keyword = IupText(NULL);
        text_source = IupText(NULL);
        text_res = IupText(NULL);

        //khai báo các nút

        btn_encrypt = IupButton("Encrypt", NULL);
        btn_descrypt = IupButton("Descrypt", NULL);
        btn_clear = IupButton("Clear", NULL);

        // khai báo các khung

        frame_encrypt = IupFrame(text_source);
        frame_keyword = IupFrame(IupHbox(text_keyword, toggle, NULL));
        frame_res = IupFrame(text_res);

        //khai báo các phần tử menu

        item_open = IupItem("Open...", NULL);
        item_saveas = IupItem("Save as...", NULL);
        item_exit = IupItem("Exit", NULL);

        //khai báo các thành phần của sub menu file

        file_menu = IupMenu(
            item_open,
            item_saveas,
            IupSeparator(),
            item_exit,
            NULL);

        sub1_menu = IupSubmenu("File", file_menu);

        //khai báo menu chính

        main_menu = IupMenu(sub1_menu, NULL);

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

        //thêm tựa đề cho các khung

        IupSetAttribute(frame_encrypt, "TITLE", "Enter plain text/cipher text here:");
        IupSetAttribute(frame_keyword, "TITLE", "Enter keyword here:");
        IupSetAttribute(frame_res, "TITLE", "Result:");

        //điều chỉnh hộp phần tử

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

        // thêm hộp phần tử vào dialog
        dlg = IupDialog(
            element_box
        );

        //điều chỉnh dialog

        IupSetAttribute(dlg, "TITLE", "Vigenère Cipher");
        IupSetAttributeHandle(dlg, "MENU", main_menu);
        IupSetAttribute(dlg, "SIZE", "THIRDx250");
        IupSetAttribute(dlg, "RESIZE", "YES");
        IupShowXY(dlg, IUP_CENTER, IUP_CENTER);

        //gán các event cho các nút

        IupSetCallback(btn_encrypt, "ACTION", (Icallback)btn_encrypt_cb);
        IupSetCallback(btn_descrypt, "ACTION", (Icallback)btn_descrypt_cb);
        IupSetCallback(btn_clear, "ACTION", (Icallback)btn_clear_cb);

        //gán event cho các nút trong menu

        IupSetCallback(item_open, "ACTION", (Icallback)btn_open_cb);
        IupSetCallback(item_exit, "ACTION", (Icallback)btn_exit_cb);

        IupMainLoop();

        IupClose();
        return EXIT_SUCCESS;
    }