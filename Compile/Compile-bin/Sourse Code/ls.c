#define _CRT_NONSTDC_NO_DEPRECATE 1//strupr等不安全函数
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define MAX_LEN 4096
#include <stdio.h>
#include <stdlib.h>
#include<Windows.h>
#include "dirent.h"
#include <io.h>

//int cal_row(char**,int,int*,int*);				//为分栏算法提供要显示的行/列数
char* filenames[4096];	//暂存目录中文件名
int file_cnt = 0;//字符串数量
int cal_row(char** filenames, int file_cnt, int* cal_col, int* col_max_arr);
HANDLE hOut;

void colorPrint(int col_max_arr, char* filename) {
	if (GetFileAttributesA(filename) & FILE_ATTRIBUTE_DIRECTORY) {
		SetConsoleTextAttribute(hOut, 0x03);
	} else {
		char* ext;
		ext = strrchr(filename, '.');
		if (ext != NULL) {
			if (!stricmp(ext, ".exe")) {
				SetConsoleTextAttribute(hOut, 0x0A);
			}
			else if (!stricmp(ext, ".bat")) {
				SetConsoleTextAttribute(hOut, 0x0C);
			}
			else if (!stricmp(ext, ".cmd")) {
				SetConsoleTextAttribute(hOut, 0x0C);
			}
			else if (!stricmp(ext, ".cpp")) {
				SetConsoleTextAttribute(hOut, 0x0B);
			}
		}
	}
    printf("%*s", col_max_arr, filename);
    SetConsoleTextAttribute(hOut, 0x07);
}


int main(int argc, char** argv)
{
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    //DIR* dirp;
    //struct dirent* direntp;

    struct _finddata_t file;
    intptr_t   hFile;
    char buf[MAX_LEN];

    if (argc == 2) {
        strcpy(buf, argv[1]);
        strcat(buf, "\\*");
    }
    else {
        strcpy(buf, "*");
    }

    //printf("%s\n",buf);
    hFile = _findfirst(buf, &file);
    if (hFile == -1) {
        printf("打开路径 %s 失败\n", argv[1]);
        return -1;
    }

    int nums = 0;
    while (_findnext(hFile, &file) == 0)
    {
        nums++;
        if (nums > 1) {
            filenames[file_cnt] = new char[MAX_LEN];
            //sprintf_s(filenames[file_cnt], MAX_LEN, "%s", file.name);
            strcpy(filenames[file_cnt], file.name);
            //printf("%s\n", filenames[file_cnt]);
            file_cnt++;
        }
    }
    _findclose(hFile);

    /*file_cnt = 0;
    nums = 0;
    while ((direntp = readdir(dirp)) != NULL)
    {
        nums++;
        if (nums > 2) {
            printf("中文：%s \n",direntp->d_name);
            filenames[file_cnt] = (char*)malloc(200);
            sprintf(filenames[file_cnt],"%s",direntp->d_name);
            strcpy(filenames[file_cnt], direntp->d_name);
            file_cnt++;
        }
    }*/

    //closedir(dirp);

    int col = 0;
    int col_max_arr[256];//每列最大字符串长度
    int row = cal_row(filenames, file_cnt, &col, col_max_arr);
    //最小行数，int base_row = cur_file_size / col;
    int num = 0;
    int colss = 0;//列数组下标


    /* printf("----\n");
    for(int i=0;i<file_cnt;i++){
        printf("%s\n",strrchr(filenames[i],'.'));
    }
    printf("----\n"); */

    /* int show_x[row];
    for(int i=0;i<row;i++) show_x[i]=0;




    for(int i=0;i<file_cnt;i++){
        if(num>=row) {
            num = 0;
            colss++ ;
            printf("---------\n");
        }
        num++;
        printf("[%d]%*s - 最大字符串长度:%d - %d列 \n",i,-44,filenames[i],col_max_arr[colss],col);
        //printf("%*s\n",-col_max_arr[colss],filenames[i]);
    } */
    //需要得到的关键：row 最小行数 col_max_arr[256] 列数col  每列最大字符串长度 filenames[4096]文件名 file_cnt 文件数量
    /* printf("行数 row:%d%  文件数量file_cnt:%d 列数col:%d\n",row,file_cnt,col);
    for(int i=0;i<col;i++){
        printf("%d  ",col_max_arr[i]);
    }
    printf("\n"); */
    int shwo = 0;//文件名数组下标
    int tt = 0;//计数
    for (int i = 0; i < file_cnt; i++) {
        //printf("[%d - %d - %d]",shwo,col_max_arr[tt],tt);
        //printf("%*s", -col_max_arr[tt], filenames[shwo]);
        colorPrint(-col_max_arr[tt], filenames[shwo]);
        shwo += row;
        tt++;
        if (tt >= col || shwo >= file_cnt) {//列
            printf("\n");
            colss++;
            shwo = colss;
            tt = 0;
        }
    }
    exit(1);
}

int cal_row(char** filenames, int file_cnt, int* cal_col, int* col_max_arr)
{
    //获取路径名中的文件名之前的长度

    int size;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    size = csbi.srWindow.Right - csbi.srWindow.Left;
    int col = size;      //获得当前窗口的宽度（字符数）
    //printf("screen_width:%d\n",col);
    int col_max = 0;
    int cur_file_size = 0;//总字符数
    int* filenames_len = new int[file_cnt];
    *cal_col = 0;
    int i = 0;
    int j = 0;
    //获得每个文件名的字符长度
    for (i = 0; i < file_cnt; ++i) {
        filenames_len[i] = strlen(filenames[i]) + 2;		//字符串至少带两个空格
        /*特殊情况：当最大字符串长度比屏幕宽度大时,直接返回行数:file_cnt,列数：1,最大宽度:最大的字符串长度*/
        if (filenames_len[i] > col) {
            *cal_col = 1;
            col_max_arr[0] = filenames_len[i];
            return file_cnt;
        }
        cur_file_size += filenames_len[i];
        //printf("filenames_len[%d]:%d \n",i,filenames_len[i]);
    }
    //最小行数，在此基础上迭代
    int base_row = cur_file_size / col;
    if (cur_file_size % col) {
        base_row++;
    }
    int flag_succeed = 0;		//标记是否排列完成
    //-----------------------------------------------------
    //开始排列
    while (!flag_succeed) {
        int remind_width = col;	//当前可用宽度
        *cal_col = -1;
        for (i = 0; i < file_cnt; ++i) {
            /*如果剩余宽度不足以容纳当前字符串，则跳出并分配额外的行空间*/
            //printf("if filenames_len[%d]:%d > 剩余宽度remind_width:%d\n",i,filenames_len[i],remind_width);
            if (filenames_len[i] > remind_width) {
                break;
            }
            //新起的一列
            if (i % base_row == 0) {
                ++(*cal_col);
                col_max_arr[*cal_col] = filenames_len[i];
            }
            else {
                col_max_arr[*cal_col] = (filenames_len[i] > col_max_arr[*cal_col]) ? filenames_len[i] : col_max_arr[*cal_col];
            }
            //最后一行，更新剩余的宽度
            if (i % base_row == base_row - 1) {
                remind_width -= col_max_arr[*cal_col];
            }
        }
        //判断是否排列完成
        //printf("[%d]剩余宽度:[%d] 最小行数:[%d]\n",i,remind_width,base_row);
        if (i == file_cnt) {
            flag_succeed = 1;
        }
        //再分配额外行空间
        else {
            int extra = 0;						//所需额外的字符数
            while (i < file_cnt) {
                //printf("所需额外的字符数:%d+=%d\n",extra,filenames_len[i]);
                extra += filenames_len[i++];
            }
            //printf("所需额外的字符数:%d %% col:%d \n最小行数:[%d] ",extra,col,base_row);
            if (extra % col) {
                base_row += (extra / col + 1);
            }
            else {
                base_row += (extra / col);
            }
            //printf("+= extra:[%d]/ col:[%d] = 最小行数:[%d] \n",extra,col,base_row);
        }

    }
    ++(*cal_col);								//列标从0开始，所以最后加1
    return base_row;
}
