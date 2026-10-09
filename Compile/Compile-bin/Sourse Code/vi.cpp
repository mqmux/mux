#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include<windows.h>
#include<stdio.h>
#include<conio.h>

int sy(0), ey(0), eey(0);

void gotoxy(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

int gotoxy(int x) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    if (x == 0)
        return csbi.dwCursorPosition.X;
    else
        return csbi.dwCursorPosition.Y;
}

void vim(char*);

int main(int argc, char* argv[])
{
    char* strs = new char[2048];
    vim(strs);

    char filename[255];
    if (argc != 2) {
        printf("\nFilename:");
        scanf("%s", &filename);
    }
    else strcpy(filename, argv[1]);
    FILE* p = fopen(filename, "r");
    char mode[2] = "w";
    if (p != NULL)
    {
        printf("\n---------------------------\n");
        char s[1024] = { 0 };
        while (fgets(s, sizeof(s), p))
            printf("%s", s);
        fclose(p);
        printf("\n---------------------------\n  - 覆盖[Y]追加[A]取消[N]:");
        char chars;
        while (1) {
            chars = _getch();
            if (chars == 'y' || chars == 'Y') {
                break;
            }
            if (chars == 'a' || chars == 'A') {
                mode[0] = 'a';
                break;
            }
            if (chars == 'n' || chars == 'N' || chars == 3) {
                printf("\n");
                return 0;
            }
        }
    }
    p = fopen(filename, mode);
    if (p == NULL)
    {
        printf("文件打开失败\n");
    }
    else
    {
        fputs(strs, p);
        fclose(p);
    }
    printf("\n");
    return 0;
}

void vim(char* long_str) {
    char input[255][1024];
    int i = 0;
    int j = 0;
    char chars;
    int max[255];
    while (1) {
        chars = _getch();
        if (chars == '\b') {
            if (i > 0) {
                printf("\b \b");
                i--;
                input[j][i] = '\0';
                max[j] = i;
            }
            else {
                if (j > 0) {
                    int tmp = gotoxy(1) - 1;
                    gotoxy(max[j - 1], tmp);
                    j--;
                    i = max[j];
                }
            }
        }
        if (chars == '\r') {
            printf("\n");
            input[j][i] = '\n';
            max[j] = i;
            j++;
            i = 0;
        }
        if (chars == '\n') {
            max[j] = i;
            break;
        }
        if (chars < 127 && chars>31 || chars < 0) {
            input[j][i] = chars;
            i++;
            printf("%c", chars);
        }
    }
    int s = 0;
    for (int m = 0; m <= j; m++) {
        for (int n = 0; n <= max[m]; n++) {
            long_str[s] = input[m][n];
            s++;
        }
    }
}
