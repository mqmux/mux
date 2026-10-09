#include <windows.h>
void MoveWindow(int posx, int posy)
{
    RECT rectClient, rectWindow;
    HWND hWnd = GetConsoleWindow();
    GetClientRect(hWnd, &rectClient);
    GetWindowRect(hWnd, &rectWindow);
    MoveWindow(hWnd, posx, posy, rectClient.right - rectClient.left, rectClient.bottom - rectClient.top, TRUE);
}

void MoveCenter()
{
    RECT rectClient, rectWindow;
    HWND hWnd = GetConsoleWindow();
    GetClientRect(hWnd, &rectClient);
    GetWindowRect(hWnd, &rectWindow);
    int posx, posy;
    posx = GetSystemMetrics(SM_CXSCREEN) / 2 - (rectWindow.right - rectWindow.left) / 2,
    posy = GetSystemMetrics(SM_CYSCREEN) / 2 - (rectWindow.bottom - rectWindow.top) / 2,

    MoveWindow(hWnd, posx, posy, rectClient.right - rectClient.left, rectClient.bottom - rectClient.top, TRUE);
}

int main(int argc, char *argv[])
{
	int Width{ GetSystemMetrics(SM_CXSCREEN) };
	int Heigth{ GetSystemMetrics(SM_CYSCREEN) };
	if(argc>1){
		int W(50),H(50);
		W=atoi(argv[1])*Width/100;
		H=atoi(argv[2])*Heigth/100;
		MoveWindow(W,H);
	} else {
		//MoveWindow(Width/2,Heigth/2);
	}
	int mouse_pos = Width;
    mouse_pos = mouse_pos << 16 | Heigth;
    return mouse_pos;
}

/* char chCmd[32];
sprintf(chCmd,"mode con cols=%d lines=%d",height,width);
system(stCmd);
 */