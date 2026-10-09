#include<windows.h>
#include <stdio.h>
int main(int argc,char *argv[])
{
	if( argc!=6 ){
		printf("Usage: %s [X] [Y] [EX] [EY] [RE]\n"
               "  [X]: 左上角起点横坐标\n"
               "  [Y]: 左上角起点竖坐标\n"
               " [EX]: 右下角起点横坐标\n"
               " [EY]: 右下角起点竖坐标\n"
               " [RE]: 点击了确定的返回值\n"
            
            "\t%%ERRORLEVEL%% 高 16 位包含鼠标 X 坐标\n"
            "\t%%ERRORLEVEL%% 低 16 位包含鼠标 Y 坐标\n"
            "比如:返回 65537, 可以用如下方式取得 X,Y\n"
            "\tset /a ret=%%errorlevel%%\n"
            "\tset /a \"x=%%ret%%>>16\"\n"
            "\tset /a \"y=%%ret%%&65535\"\n\n",argv[0]);
			return 0;
	}
	//HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	DWORD dwMode;
	//DWORD dwOldMode;
	
	//GetConsoleMode(hIn, &dwOldMode);    /* 取得控制台原来的模式 */
    //dwMode = dwOldMode;
	GetConsoleMode(hIn, &dwMode);
    dwMode &= ~(ENABLE_QUICK_EDIT_MODE);
    dwMode |= ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	
	INPUT_RECORD mouseRec;
	DWORD res;
	SetConsoleMode(hIn, dwMode);
		ReadConsoleInput(hIn, &mouseRec, 1, &res);//读取一个鼠标操作
		int MX=mouseRec.Event.MouseEvent.dwMousePosition.X;
		int MY=mouseRec.Event.MouseEvent.dwMousePosition.Y;
		int mouse_pos;
		mouse_pos = MX;
        mouse_pos = mouse_pos << 16 | MY;
		if( MX>=atoi(argv[1]) && MX<=atoi(argv[3])+1 && MY>=atoi(argv[2]) && MY<=atoi(argv[4]) ) {
			printf("%d %d ",MX-atoi(argv[1]),MY-atoi(argv[2]));
			if(mouseRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				printf("%d    \r",atoi(argv[5]));
			} else {
				printf("0    \r");
			}
		}
		/* if( mouseRec.EventType == MOUSE_EVENT ) {
			
		} */
        return mouse_pos;
		//printf("%d,%d      \r",mouseRec.Event.MouseEvent.dwMousePosition.X,mouseRec.Event.MouseEvent.dwMousePosition.Y);
	//SetConsoleMode(hIn, dwOldMode);    // 还原原来的设置 
	//CloseHandle(hOut);  // 关闭标准输出设备句柄  
	//CloseHandle(hIn);   // 关闭标准输入设备句柄  
	//return 0;
}
