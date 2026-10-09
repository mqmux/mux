#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include<iostream>
#include <string>
#include <bitset>
//#include <sstream>
//#include <iterator>

using namespace std;

string string_to_hex(const std::string& input)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}
//转字符串
string hex_to_string(const std::string& str)
{
	std::string result;
	for (size_t i = 0; i < str.length(); i += 2)
	{
		std::string byte = str.substr(i, 2);
		char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
		result.push_back(chr);
	}
	return result;
}

int main(int argc, char* argv[])
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (argc > 1) {
		char* stop = new char[20];
		WORD w = strtol(argv[1], &stop, 16);
		WORD w1 = strtol(argv[2], &stop, 16);
		if (!strcmp("-n", argv[1])) {
			SetConsoleTextAttribute(hOut, w1);
			for (int j = 1; j <= atoi(argv[3]); j++) {
				for (int i = 4; i < argc; i++) {
					printf("%s", argv[i]);
				}
			}
			SetConsoleTextAttribute(hOut, 0x07);
			return 0;
		}
		if (!strcmp("-u", argv[1])) {
			SetConsoleTextAttribute(hOut, w1 | COMMON_LVB_UNDERSCORE);
			printf("%s", argv[3]);
			SetConsoleTextAttribute(hOut, 0x07);
			return 0;
		}
		if (!strcmp("-t", argv[1])) {
			SetConsoleTextAttribute(hOut, w1);
			string s = argv[4];
			size_t COUNT = s.length();
			for (size_t i(0); i < COUNT; i += 1)
			{
				cout << s.substr(i, 1);
				Sleep(atoi(argv[3]));
			}
			SetConsoleTextAttribute(hOut, 0x07);
			return 0;
		}
		if (!strcmp("-a", argv[1])) {
			long ans = strtol(argv[3], &stop, atoi(argv[2]));
			printf("HEX: %X\n", ans);
			printf("DEC: %d\n", ans);
			cout << "OCT: " << oct << ans << endl;
			//char s[16];
			//itoa(ans, s, 2);
			//printf("BIN %s\n", s);
			cout << "BIN: " << bitset<32>(ans) << endl;
			return 0;
		}
		if (!strcmp("-h", argv[1])) {
			cout << string_to_hex(argv[2]) << endl;
			return 0;
		}
		SetConsoleTextAttribute(hOut, w);
		for (int i = 2; i < argc; i++) {
			printf("%s", argv[i]);
		}
		SetConsoleTextAttribute(hOut, 0x07);
		return 0;
	}
	else {
		/* printf("Usage: %s [color] [str] \n",argv[0]);
		printf("   or: %s [-n] [color] [number] [str] 设置显示字符串次数number \n",argv[0]);
		printf("   or: %s [-u] [color] [str] 下划线显示 \n");
		printf("   or: %s [-t] [color] [time] [str] 设置显示每个字符中间间隔时间time \n",argv[0]);
		printf("   or: %s [-a] [scale] [number] 进制[2-36]进制,数字 \n",argv[0]);
		printf("   or: %s 无参数显示此帮助信息 \n\n",argv[0]);
		printf("   color 字体颜色,详细参照color /? \n"); */
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		GetCurrentConsoleFontEx(hOut, 0, &cfi);
		/* cout << "\n   字体: " << cfi.FaceName;
		cout << endl;
		cout << "   大小: " << cfi.dwFontSize.X << "x" << cfi.dwFontSize.Y << endl
			 << "   权重: " << cfi.FontWeight << endl; */

		cfi.nFont = 0;
		cfi.dwFontSize.X = 8;           // 更改字体宽度
		cfi.dwFontSize.Y = 16;          // 更改字体高度
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"黑体"); // 更改字体名
		SetCurrentConsoleFontEx(hOut, FALSE, &cfi); // 应用更改
		return 0;
	}
}

