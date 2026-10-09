#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
int main(int argc,char* argv[])
{
	if (argc != 2) return 0;
	int bit = 10;
	if (argv[1][0] == '0' && argv[1][1] == 'x' || argv[1][1] == 'X') bit = 16;
	if (argv[1][0] == '0' && argv[1][1] != 'x' && argv[1][1] != 'X') bit = 8;
	char* stop;
	int ans = strtol(argv[1], &stop, bit);   //将八进制数1054转成十进制，后面均为非法字符 printf("%s\n", stop);
	char str[100];
	_itoa(ans, str, 2);  //c++中一般用_itoa，用itoa也行,
	printf("HEX:%X DEC:%d OCT:%o BIN:%s",ans, ans, ans, str);
	return 0;
}