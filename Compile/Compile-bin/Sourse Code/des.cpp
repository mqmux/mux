#include <iostream>
#include <string>
#include <cmath>
#include <bitset>
#include <iomanip>
#include <sstream>

using namespace std;

string roundKey[16];//函数在传递数组的时候需要用到指针，因为不想使用指针，所以一开始预定义

//生成轮密钥的过程//
//1：生成密钥置换PC-1，将输入的初始密钥由64位变成56位
int PC1[] = { 57, 49, 41, 33, 25, 17, 9,
			   1, 58, 50, 42, 34, 26, 18,
			  10,  2, 59, 51, 43, 35, 27,
			  19, 11,  3, 60, 52, 44, 36,
			  63, 55, 47, 39, 31, 23, 15,
			   7, 62, 54, 46, 38, 30, 22,
			  14,  6, 61, 53, 45, 37, 29,
			  21, 13,  5, 28, 20, 12,  4 };
string PC1Exchange(string key) {//PC-1置换函数
	string result = "";
	for (int i = 0; i < 56; ++i) {
		result += key[PC1[i] - 1];
	}
	return result;
}
//2：生成的56比特分为C0和的D0,每一轮生成的Ci和Di都要进行循环左移，循环左移轮数写成一张表
int LS[] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
string LeftShift(string key, int ShiftNumber) {//循环左移函数
	return key.substr(ShiftNumber) + key.substr(0, ShiftNumber);
}
//3：生成压缩置换表，将Ci和Di合并为56位密钥，然后进行压缩，生成48位轮密钥
int PC2[] = { 14, 17, 11, 24,  1,  5,
			   3, 28, 15,  6, 21, 10,
			  23, 19, 12,  4, 26,  8,
			  16,  7, 27, 20, 13,  2,
			  41, 52, 31, 37, 47, 55,
			  30, 40, 51, 45, 33, 48,
			  44, 49, 39, 56, 34, 53,
			  46, 42, 50, 36, 29, 32 };
string PC2Exchange(string key) {//压缩置换PC-2函数
	string result = "";
	for (int i = 0; i < 48; ++i) {
		result += key[PC2[i] - 1];
	}
	return result;
}
//轮密钥生成函数
void KeyGenerate(string initKey) {//将初始密钥传递到函数中
	string leftKey;//将密钥分为Ci和Di,分别用leftKey和rightKey表示
	string rightKey;
	string tempKey = PC1Exchange(initKey);//用来存储经过密钥置换PC-1之后的56位密钥值以及生成的密钥中间值
	for (int i = 0; i < 16; ++i) {
		leftKey = tempKey.substr(0, 28);//将56位密钥分为左右两部分
		rightKey = tempKey.substr(28);
		leftKey = LeftShift(leftKey, LS[i]);//对左右两部分Ci和Di进行循环左移操作
		rightKey = LeftShift(rightKey, LS[i]);
		tempKey = leftKey + rightKey;//将Ci和Di进行合并得到密钥中间值
		roundKey[i] = PC2Exchange(tempKey);
	}
}
//生成轮密钥的过程结束//

//加解密操作的过程//
//1：初始置换IP
int IP[] = { 58, 50, 42, 34, 26, 18, 10, 2,
			 60, 52, 44, 36, 28, 20, 12, 4,
			 62, 54, 46, 38, 30, 22, 14, 6,
			 64, 56, 48, 40, 32, 24, 16, 8,
			 57, 49, 41, 33, 25, 17, 9,  1,
			 59, 51, 43, 35, 27, 19, 11, 3,
			 61, 53, 45, 37, 29, 21, 13, 5,
			 63, 55, 47, 39, 31, 23, 15, 7 };
string IPExchange(string init) {//IP置换函数
	string result = "";
	for (int i = 0; i < 64; ++i) {
		result += init[IP[i] - 1];
	}
	return result;
}
//2：函数F
//扩展置换E
int E[] = { 32,  1,  2,  3,  4,  5,
			4,  5,  6,  7,  8,  9,
			8,  9, 10, 11, 12, 13,
			12, 13, 14, 15, 16, 17,
			16, 17, 18, 19, 20, 21,
			20, 21, 22, 23, 24, 25,
			24, 25, 26, 27, 28, 29,
			28, 29, 30, 31, 32,  1 };
string EExchange(string right) {//扩展置换函数
	string result = "";
	for (int i = 0; i < 48; ++i) {
		result += right[E[i] - 1];
	}
	return result;
}
//S盒
int SBOX[8][4][16] = {
	{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
	 {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
	 {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
	 {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
	{{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
	 {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
	 {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
	 {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
	{{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
	 {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
	 {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
	 {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
	{{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
	 {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
	 {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
	 {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
	{{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
	 {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
	 {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
	 {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
	{{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
	 {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
	 {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
	 {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
	{{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
	 {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
	 {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
	 {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
	{{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
	 {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
	 {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
	 {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}
};
string DecToBin(int str) {//十进制转二进制的函数实现
	string bin = "";
	while (str >= 1) {
		bin = to_string(str % 2) + bin;
		str = str / 2;
	}
	while (bin.size() != 4) {
		bin = '0' + bin;
	}
	return bin;
}
int BinToDec(char str) {//二进制转十进制的函数实现
	return str - '0';
}
string SExchange(string right) {//S盒置换函数
	string result = "";
	for (int i = 0; i < 48; i = i + 6) {
		int row = BinToDec(right[i]) * 2 + BinToDec(right[i + 5]);
		int col = BinToDec(right[i + 1]) * 8 + BinToDec(right[i + 2]) * 4 + BinToDec(right[i + 3]) * 2 + BinToDec(right[i + 4]);
		result += DecToBin(SBOX[i / 6][row][col]);
	}
	return result;
}
//P置换
int P[] = { 16,  7, 20, 21,
			29, 12, 28, 17,
			1, 15, 23, 26,
			5, 18, 31, 10,
			2,  8, 24, 14,
			32, 27, 3,  9,
			19, 13, 30, 6,
			22, 11,  4, 25 };
string PExchange(string right) {//P置换函数
	string result = "";
	for (int i = 0; i < 32; ++i) {
		result += right[P[i] - 1];
	}
	return result;
}
string XOR(string str1, string str2) {//异或函数实现
	string res = "";
	for (int i = 0; i < str1.size(); i++) {
		if (str1[i] == str2[i]) {
			res += "0";
		}
		else {
			res += "1";
		}
	}
	return res;
}
//函数F函数实现
string F(string right, string key) {
	string temp = EExchange(right);//用来存储right部分进行扩展置换后的中间值
	temp = XOR(temp, key);//进行异或操作
	string result = SExchange(temp);//进行S盒转换
	result = PExchange(result);//进行P置换
	return result;
}
//3：尾置换IP逆
int IPR[] = { 40, 8, 48, 16, 56, 24, 64, 32,
			  39, 7, 47, 15, 55, 23, 63, 31,
			  38, 6, 46, 14, 54, 22, 62, 30,
			  37, 5, 45, 13, 53, 21, 61, 29,
			  36, 4, 44, 12, 52, 20, 60, 28,
			  35, 3, 43, 11, 51, 19, 59, 27,
			  34, 2, 42, 10, 50, 18, 58, 26,
			  33, 1, 41,  9, 49, 17, 57, 25 };
string IPRExchange(string end) {//IP逆置换实现
	string result = "";
	for (int i = 0; i < 64; ++i) {
		result += end[IPR[i] - 1];
	}
	return result;
}
//4：加密函数实现
string encrypt(string plain, string key) {
	string cipher = "";
	string temp = IPExchange(plain);//对输入明文进行初始置换IP
	string left = temp.substr(0, 32);//将经过IP置换的明文串转换为左右两部分
	string right = temp.substr(32);
	for (int i = 0; i < 16; i++) {//进行16轮迭代
		string middle = right;
		right = XOR(left, F(right, roundKey[i]));
		left = middle;
	}
	cipher = right + left;//对左右两部分进行合并
	cipher = IPRExchange(cipher);//进行IP逆置换
	return cipher;
}
//5：解密函数实现
string decrypt(string cipher, string key) {
	string plain = "";
	string temp = IPExchange(cipher);//对输入密文进行初始置换IP
	string left = temp.substr(0, 32);//将经过IP置换的密文串转换为左右两部分
	string right = temp.substr(32);
	for (int i = 0; i < 16; ++i) {//进行16轮迭代
		string middle = right;
		right = XOR(left, F(right, roundKey[15 - i]));
		left = middle;
	}
	plain = right + left;//对左右两部分进行合并
	plain = IPRExchange(plain);//进行IP逆置换
	return plain;
}
int HexToDec(char str) {//十六进制转换为十进制的函数实现
	int dec = 0;
	if (str >= 'A' && str <= 'F') {
		dec += (str - 'A' + 10);
	}
	else {
		dec += (str - '0');
	}
	return dec;
}
//加解密函数实现结束

string HexToBin(string str) {//十六进制转换为二进制的函数实现
	string bin = "";
	string table[16] = { "0000","0001","0010","0011","0100","0101","0110","0111","1000","1001","1010","1011","1100","1101","1110","1111" };
	for (int i = 0; i < str.size(); i++) {
		if (str[i] >= 'A' && str[i] <= 'F') {
			bin += table[str[i] - 'A' + 10];
		}
		else {
			bin += table[str[i] - '0'];
		}
	}
	return bin;
}
string BinToHex(string str) {//二进制转换为十六进制的函数实现
	string hex = "";
	int temp = 0;
	while (str.size() % 4 != 0) {
		str = "0" + str;
	}
	for (int i = 0; i < str.size(); i += 4) {
		temp = (str[i] - '0') * 8 + (str[i + 1] - '0') * 4 + (str[i + 2] - '0') * 2 + (str[i + 3] - '0') * 1;
		if (temp < 10) {
			hex += to_string(temp);
		}
		else {
			hex += 'A' + (temp - 10);
		}
	}
	return hex;
}
string getPaddingString(string const& str, int n)
{
	ostringstream oss;
	oss << setw(n) << setfill('c') << str;
	return oss.str();
}
int main() {
	string input = "893d9e85037b401fbb002c8f50b058a3";
	string initKey = "CCTV";
	int n = input.size();
	initKey = getPaddingString(initKey, n);
	cout << "\'" << input << "\'\n";
	cout << "\'" << initKey << "\'\n";
	string plain = HexToBin(input);
	string key = HexToBin(initKey);
	KeyGenerate(key);
	string cipher = encrypt(plain, key);
	cout << "明文：   " << input << endl;
	cout << "密钥：   " << initKey << endl;
	cout << "密文：   " << cipher << endl;
	string output = decrypt(cipher, key);
	cout << "密文：   " << cipher << endl;
	cout << "密钥：   " << initKey << endl;
	cout << "明文：   " << input << endl;
}
