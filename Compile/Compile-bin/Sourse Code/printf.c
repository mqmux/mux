#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include<iostream>
#include <string>
#include <bitset>
#include <cstdlib>
#include <cstring>

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
		if (argc < 3) {
			printf("Not enough arguments\n");
			return 1;
		}

		char* stop = new char[20];
		WORD w = strtol(argv[1], &stop, 16);
		WORD w1 = strtol(argv[2], &stop, 16);

		if (!strcmp("-n", argv[1])) {
			if (argc < 4) {
				printf("Not enough arguments for -n option\n");
				return 1;
			}
			int num = atoi(argv[3]);
			if (num < 0) {
				printf("Invalid number for -n option\n");
				return 1;
			}
			SetConsoleTextAttribute(hOut, w1);
			for (int j = 0; j < num; j++) {
				for (int i = 4; i < argc; i++) {
					printf("%s", argv[i]);
				}
			}
			SetConsoleTextAttribute(hOut, 0x07);
			return 0;
		}

		// ... similar checks for other options ...

		SetConsoleTextAttribute(hOut, w);
		for (int i = 2; i < argc; i++) {
			printf("%s", argv[i]);
		}
		SetConsoleTextAttribute(hOut, 0x07);
		return 0;
	}
	else {
		// ... original code ...
	}
}