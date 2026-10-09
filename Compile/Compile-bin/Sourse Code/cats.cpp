# include <iostream>
# include <fstream>
#include <sstream>
using namespace std;

typedef std::string			C_String;	//新类型
typedef std::ifstream		C_Ifstream;
typedef std::stringstream	C_StringStream;

const int KEYWORDS_LENGTH = 22;
C_String keywords[KEYWORDS_LENGTH] = {
    "switch", "default", "case", "break", "const", "string", "struct" ,"int","char","return","if","using","namespace","typedef","for","bool","void","else","while","sizeof","continue","NULL"
};

bool IsUpper(const char& ch_char) {			//大写
    return ch_char >= 'A' && ch_char <= 'Z';
}

bool IsLower(const char& ch_char) {			//小写
    return ch_char >= 'a' && ch_char <= 'z';
}

bool IsLetter(const char& ch_char) {			//字母
    return IsUpper(ch_char) || IsLower(ch_char);
}

bool IsNumber(const char& ch_char) {			//数字
    return (ch_char >= '0' && ch_char <= '9') || ch_char == '.';
}

bool IsIdentifier(const char& ch_char) {		//后标识符
    return IsLetter(ch_char) || IsNumber(ch_char) || ch_char == '_';
}

bool IsStringInStrings(const C_String& o_string, C_String ao_strings[], const int& stringsLength) {
    for (unsigned int index = 0; index < stringsLength; index++) {
        if (ao_strings[index] == o_string) return true;
    }return false;
}

#include <windows.h>
void setColor(int color);

void Analyze(C_String text) {
    //缓冲字符串
    C_String o_bufferSave = "";

    for (unsigned int index = 0; index < text.length(); index++) {
        //缓冲字符串清空
        o_bufferSave = "";

        //是字母或是下划线
        if (IsLetter(text[index]) || text[index] == '_') {
            //获取这个单词
            while (IsIdentifier(text[index])) {
                //将单词内容逐个塞入缓冲字符串
                o_bufferSave = o_bufferSave + text[index];
                index++;
            }index--;//退位

            //是关键字
            if (IsStringInStrings(o_bufferSave, keywords, KEYWORDS_LENGTH)) {
                //打印信息
                setColor(0x03);
                cout << o_bufferSave;
                setColor(0x07);
                //不是
            }
            else {
                //打印信息
                cout << o_bufferSave;
            }
        }
        else if (IsNumber(text[index])) {//是数字
         //获取整个数字
            while (IsNumber(text[index])) {
                //将数字逐一塞到缓冲字符串
                o_bufferSave = o_bufferSave + text[index];
                index++;
            }index--;//退位

            //打印信息
            setColor(0x02);
            cout << o_bufferSave;
            setColor(0x07);
        }
        else {//符号处理
            int bufferTotal = 1;
            switch (text[index]) {
            case '\'':
                //向缓冲字符串塞入第一个单引号
                o_bufferSave += '\'';
                //挪到下一位
                index++;
                //获取单引号之间的内容塞到缓冲字符串里面
                while (1) {
                    o_bufferSave += text[index];
                    if (text[index] == '\\') {
                        index++;
                        o_bufferSave += text[index];
                        index++;
                        continue;
                    }
                    if (text[index] == '\'') {
                        //index++;
                        break;
                    }
                    index++;
                }
                //o_bufferSave += text[index];//再将第二格单引号塞入缓冲字符串
                //打印信息
                setColor(0x06);
                cout << o_bufferSave;
                setColor(0x07);

                break;
                //同上
            case '"':
                o_bufferSave += '"';
                index++;
                while (text[index] != '"') {
                    o_bufferSave += text[index];
                    index++;
                }o_bufferSave += text[index];
                setColor(0x06);
                cout << o_bufferSave;
                setColor(0x07);

                break;
            case '/':
                if (text[index + 1] == '/') {
                    o_bufferSave += '/';
                    index++;
                    while (text[index] != '\n') {
                        o_bufferSave += text[index];
                        index++;
                    }o_bufferSave += text[index];
                    setColor(0x0a);
                    cout << o_bufferSave;
                    setColor(0x07);
                }
                if (text[index + 1] == '*') {
                    o_bufferSave += '/';
                    index++;
                    while (1) {
                        o_bufferSave += text[index];
                        index++;
                        if (text[index] == '*' && text[index + 1] == '/') {
                            o_bufferSave += text[index];
                            index++;
                            break;
                        }
                    }o_bufferSave += text[index];
                    setColor(0x0a);
                    cout << o_bufferSave;
                    setColor(0x07);
                }

                break;
                //其他
            default:
                cout << text[index];
                break;
            }
        }
    }

    return;
}


int main(int args, char* argv[]) {

    /*
     *	文件路径
     *	文件读取流
     *	文件内容存放处
     *	字符串流
     */
    if (args != 2) {
        cout << "Usage: " << argv[0] << " [filename]" << endl;
        return 0;
    }
    char			ach_filePath[100];//此处换成要分析的文件路径
    strcpy(ach_filePath, argv[1]);
    ifstream		o_fileInput(ach_filePath);
    C_String		o_fileText = "";
    C_StringStream	o_stringstream;

    if (!o_fileInput) {
        cout << "文件打开失败";
    }
    else {
        //字符流读取文件内容，包括空格换行
        o_stringstream << o_fileInput.rdbuf();
        //字符流将内容导入文件内容存放处，包括空格和换行
        o_fileText = o_stringstream.str();

        //分析
        Analyze(o_fileText);

    }
    return 0;
}

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
