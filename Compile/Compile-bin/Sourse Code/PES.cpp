
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream> 
#include <cctype>
#include <cstdlib>
#include<sstream>
using namespace std;
int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <filepath>  [wide] " << std::endl;
        exit(1);
    }

    int wide = argc > 2 ? std::stoi(std::string(argv[2])) : 16; //每行的字节数

    const int buf_size = 1024;
    uint8_t* buf = new uint8_t[buf_size];

    std::ifstream in;
    in.open(argv[1]);
    if (!in.is_open()) {
        std::cerr << "cannot open the file" << std::endl;
        exit(1);
    }

    //十六进制输出
    std::cout << std::hex;
    std::cout << std::setfill('0');

    int printed = 0; //下一个要输出的字节

    stringstream display; //每行字节序列的可打印形式

    while (in.good()) {

        in.read(reinterpret_cast<char*>(buf), buf_size);
        int readnum = in.gcount();

        for (unsigned i = 0; i < readnum; i++) {
            //是否是一行开头
            if (printed % wide == 0) {
                std::cout << std::setw(8) << printed << ":   ";
            }
            //输出一个字节
            std::cout << std::setw(2) << 0 + buf[i];
            //输出的字节对应的打印形式保存到display
            display << (static_cast<bool>(isprint(static_cast<char>(buf[i]))) ? static_cast<char>(buf[i]) : '.');
            //是否为两个字节为一组中的最后一个（没到行末）
            if (printed % 2 != 0 && printed % wide != (wide - 1)) {
                std::cout << ' ';
            }
            else if (printed % wide == (wide - 1)) { // 行末
                std::cout << "    " << display.str() << '\n';
                display.str(""); //重置display
            }

            printed++;

        }

    }

    // 读到文件末尾

    if (printed % wide != 0) { //若最后一行没有满，要补上该行的打印形式
        int byte_num = wide - printed % wide; //最后一行剩余可打印的字节数
        int ws_num = 4 + byte_num / 2 + 2 * byte_num + (byte_num % 2 == 0 ? -1 : 0);  //行末固定的分隔+ 两个字节之间的空白+ 未打印的字节序列所占空白 + 根据剩下字节数对分隔空白的修正
        std::cout << std::string(ws_num, ' ') << display.str() << std::endl;
    }
}


