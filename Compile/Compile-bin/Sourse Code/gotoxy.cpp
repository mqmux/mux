#include <stdio.h>
#include <windows.h>
using namespace std;

int main(int argc,char *argv[])
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//COORD coordScreen = {0, 0}; //¹â±êÎ»ÖÃ
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	if(argc>1){
		if(!strcmp(argv[1],"-l")){
			COORD coord;
			if ( atoi(argv[2]) < 0 ) {
			coord.X = csbi.dwCursorPosition.X;
			} else coord.X = atoi(argv[2]);
			
			if ( atoi(argv[3]) < 0 ) {
				coord.Y = csbi.dwCursorPosition.Y;
			} else coord.Y = atoi(argv[3]);
			
			SetConsoleCursorPosition(hConsole, coord);
			
		} else if(!strcmp(argv[1],"-r")) {
			int pos;
			pos = csbi.dwCursorPosition.X;
			pos = pos << 16 | csbi.dwCursorPosition.Y;
			return pos;
		} else {
			COORD coord;
			coord.X = csbi.dwCursorPosition.X + atoi(argv[1]);
			coord.Y = csbi.dwCursorPosition.Y + atoi(argv[2]);
			SetConsoleCursorPosition(hConsole, coord);
		}
	} else {
		return csbi.dwCursorPosition.X;
	}
	return csbi.dwCursorPosition.Y;
}
