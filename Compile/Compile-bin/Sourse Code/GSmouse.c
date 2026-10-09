#include <windows.h>
//#include <stdio.h>
#define HELP "x,y,[1->左键按下|2->右键按下],[1->左键起来|2->右键起来]"

#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 0:1) //必要的，我是背下来的 

int main(int argc, char** argv)
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);  
    DWORD mode,oldmode;  
    GetConsoleMode(hStdin, &oldmode); 
	mode = oldmode;
    mode &= ~ENABLE_QUICK_EDIT_MODE;  //移除快速编辑模式
    mode &= ~ENABLE_INSERT_MODE;      //移除插入模式
    mode &= ~ENABLE_MOUSE_INPUT;
    SetConsoleMode(hStdin, mode);
	POINT p;
	GetCursorPos(&p);//获取鼠标坐标
	int mouse_pos = p.x;
    mouse_pos = mouse_pos << 16 | p.y;
	if( argc == 5 ) {
		int xx = p.x;
		int yy = p.y;
		if( atoi(argv[1]) > 0 ) xx = atoi(argv[1]);
		if( atoi(argv[2]) > 0 ) yy = atoi(argv[2]);
			SetCursorPos(xx,yy);//更改鼠标坐标
		if( atoi(argv[3]) == 1 ){
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		} else if( atoi(argv[3]) == 2 ){
			mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
		}
			if( atoi(argv[4]) == 1 ) {
				Sleep(10);//要留给某些应用的反应时间
				mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
			}
			if( atoi(argv[4]) == 2 ) {
				Sleep(10); 
				mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
			}
	}
    SetConsoleMode(hStdin, oldmode);
	return mouse_pos;
}
