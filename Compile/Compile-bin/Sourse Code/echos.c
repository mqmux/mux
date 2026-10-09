#include <stdio.h>
#include <windows.h>
int main(int argc,char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s [color] [str] \n",argv[0]);
		printf("   or: %s [-f] [color] [length] [Pos_X] [Pos_Y] 着色起点坐标 \n",argv[0]);
		printf("   or: %s [-u] [color] [str] 下划线显示 \n",argv[0]);
		return 0;
	}
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	char *p = (char *)malloc(20);
	char Fill[] = "-f";
	WORD w = strtol(argv[1], &p, 16);
	WORD w1 = strtol(argv[2], &p, 16);
	if (!strcmp(Fill, argv[1])) {
		DWORD getWrittenCount = 0;
		// 着色起点坐标
		COORD writtenPos = { atoi(argv[4]), atoi(argv[5]) };
		// 着色
		FillConsoleOutputAttribute(hOut, w1, atoi(argv[3]), writtenPos, &getWrittenCount);
		return getWrittenCount;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),w);
	for(int i=2;i<argc;i++){
		printf("%s ",argv[i]);
	}
	printf("\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07);
	return 0;
}
