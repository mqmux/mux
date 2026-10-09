#include<stdio.h>
#include<Windows.h>
#include<conio.h>

int cal_row(char**,int,int*,int*);				//为分栏算法提供要显示的行/列数
char* filenames[4096];	//暂存目录中文件名
int file_cnt = 0;//字符串数量

int cal_row(char** filenames,int file_cnt,int* cal_col,int* col_max_arr);

int main(int argc,char *argv[])
{
	HWND hWnd = GetForegroundWindow();
	//printf("%#X -- [GetForegroundWindow] \n",hWnd);
		//得到桌面窗口  
		HWND hd=GetDesktopWindow();  
		//得到屏幕上第一个子窗口  
		hd=GetWindow(hd,GW_CHILD);  
		char s[100]={0};  
		//循环得到所有的子窗口  
		while(hd!=NULL)  
		{  
			memset(s,0,100);  
			GetWindowText(hd,s,100);  
			//if (strstr(s,argv[1]))  
			//{  
				//cout<<s<<endl;  
				//SetWindowText(hd,"VCC");  
				//SendMessageA(hd,WM_CLOSE,0,0);
				//ShowWindow(hd, SW_HIDE);//隐藏
			//}
			//filenames[0]=s;
			//printf("%#X %s\n",hd,s);
			filenames[file_cnt]=(char *)malloc(200);
			sprintf(filenames[file_cnt],"%X %s",hd,s);
			
			
			//snprintf(filenames[file_cnt],25,"%#X",g_arrList[i]);
			
			//printf(" - %s %d\n",filenames[file_cnt],file_cnt);
			
			hd=GetNextWindow(hd,GW_HWNDNEXT);  
			file_cnt++;
		}
		//printf("%d\n",file_cnt);
		/* for(int z=0;z<file_cnt;z++){
			printf("%s %d\n",filenames[z],z);
		} */
		
	int col = 0;
	int col_max_arr[256];
	int row = cal_row(filenames,file_cnt,&col,col_max_arr);
	//最小行数，int base_row = cur_file_size / col;
	int num=0;
	int colss=0;
	
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
	
	int shwo=0;
	colss=0;
	int tt=0;
	for(int i=0;i<file_cnt;i++){
		tt++;
		//printf("[%d - %d - %d]",shwo,col_max_arr[tt-1],tt-1);
		printf("%*s",-col_max_arr[tt-1],filenames[shwo]);
		shwo+=row;
		if( tt>=col || shwo+1 > file_cnt ){//列
			printf("\n");
			colss++;
			shwo=colss;
			tt=0;
		}
	}
	return 0;
}

int cal_row(char** filenames,int file_cnt,int* cal_col,int* col_max_arr)
{
    //获取路径名中的文件名之前的长度
    
	int size;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	size = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int col = size;      //获得当前窗口的宽度（字符数）
	//printf("screen_width:%d\n",col);
    int col_max = 0;
    int cur_file_size = 0;//总字符数
	int filenames_len[file_cnt];
	*cal_col = 0;
	int i = 0;
	int j = 0;
	//获得每个文件名的字符长度
	for(i = 0;i < file_cnt;++i){
		filenames_len[i] = strlen(filenames[i]) + 2;		//字符串至少带两个空格
		/*特殊情况：当最大字符串长度比屏幕宽度大时,直接返回行数:file_cnt,列数：1,最大宽度:最大的字符串长度*/
		if(filenames_len[i] > col){
			*cal_col = 1;
			col_max_arr[0] = filenames_len[i];
			return file_cnt;
		}
		cur_file_size += filenames_len[i];
		//printf("filenames_len[%d]:%d \n",i,filenames_len[i]);
	}
	//最小行数，在此基础上迭代
	int base_row = cur_file_size / col;
	if(cur_file_size % col){
		base_row++;
	}
	int flag_succeed = 0;		//标记是否排列完成
	//-----------------------------------------------------
	//开始排列
	while(!flag_succeed){
		int remind_width = col;	//当前可用宽度
		*cal_col = -1;
		for(i = 0;i < file_cnt;++i){
			/*如果剩余宽度不足以容纳当前字符串，则跳出并分配额外的行空间*/
			//printf("if filenames_len[%d]:%d > 剩余宽度remind_width:%d\n",i,filenames_len[i],remind_width);
			if(filenames_len[i] > remind_width){
				break;
			}
			//新起的一列
			if(i % base_row == 0){
				++(*cal_col);
				col_max_arr[*cal_col] = filenames_len[i];
			} else {
				col_max_arr[*cal_col] = (filenames_len[i] > col_max_arr[*cal_col]) ? filenames_len[i] : col_max_arr[*cal_col];
			}
			//最后一行，更新剩余的宽度
			if(i % base_row == base_row - 1){
				remind_width -= col_max_arr[*cal_col];
			}
		}
		//判断是否排列完成
		//printf("[%d]剩余宽度:[%d] 最小行数:[%d]\n",i,remind_width,base_row);
		if(i == file_cnt){
			flag_succeed = 1;
		}
		//再分配额外行空间
		else {
			int extra = 0;						//所需额外的字符数
			while(i < file_cnt){
				//printf("所需额外的字符数:%d+=%d\n",extra,filenames_len[i]);
				extra += filenames_len[i++];
			}
			//printf("所需额外的字符数:%d %% col:%d \n最小行数:[%d] ",extra,col,base_row);
			if(extra % col){
				base_row += (extra / col + 1);
			} else {
				base_row += (extra / col);
			}
			//printf("+= extra:[%d]/ col:[%d] = 最小行数:[%d] \n",extra,col,base_row);
		}
		
	}
	++(*cal_col);								//列标从0开始，所以最后加1
	return base_row;
}

