#include <Windows.h>
#include <stdio.h>

int main(const int argc, char* argv[])
{
	if(argc != 7){
		printf("Usage: %s [x] [y] [fx] [fy] 移动文本\n",argv[0]);
		return 404;
	}
	// 输出句柄
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// 获取屏幕缓冲区信息
	CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
	GetConsoleScreenBufferInfo(outputHandle, &consoleScreenBufferInfo);

	// 移动文本
	// 被移动的区域
	SMALL_RECT movePos{ atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) };
	// 移动到的新位置的左上角坐标
	//             X  Y
	COORD newPos{ atoi(argv[5]), atoi(argv[6]) };
	// 被移动区域填充的字符的信息
	CHAR_INFO charInfo;
	// 被移动区域留空
	charInfo.Char.AsciiChar = ' ';
	// 设置被移动区域新填充的字符的属性(也可以说成颜色，但是不太准确)
	// 属性的值为当前输出的文本的属性
	charInfo.Attributes = consoleScreenBufferInfo.wAttributes;

	// 调用 API
	ScrollConsoleScreenBufferA(outputHandle, &movePos, NULL, newPos, &charInfo);
	
	return 0;
}