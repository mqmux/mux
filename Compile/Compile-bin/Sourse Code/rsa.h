#ifndef RSA_H
#define RSA_H

#define max(a, b) (((a) > (b)) ? (a) : (b))
#include <string>
// PKCS#1 v1.5填充
std::string pad(const std::string& msg, int length) {
    std::string padded = "00" + msg;
    while (padded.size() < length) {
        padded = "FF" + padded;
    }
    return "00" + padded;
}

// PKCS#1 v1.5去填充
std::string unpad(const std::string& msg) {
    int i = 2;
    while (msg[i] == 'F') {
        ++i;
    }
    return msg.substr(i + 2);
}

// 大数乘法
std::string multiply(const std::string& a, const std::string& b) {
    std::string result(a.size() + b.size(), '0');
    for (int i = a.size() - 1; i >= 0; --i) {
        int carry = 0;
        for (int j = b.size() - 1; j >= 0; --j) {
            int temp = (result[i + j + 1] - '0') + (a[i] - '0') * (b[j] - '0') + carry;
            result[i + j + 1] = temp % 10 + '0';
            carry = temp / 10;
        }
        result[i] += carry;
    }
    return result;
}

// 大数模运算
std::string mod(const std::string& a, const std::string& b) {
    std::string result = a;
    while (result >= b) {
        for (int i = 0; i <= result.size() - b.size(); ++i) {
            std::string temp = b + std::string(i, '0');
            if (result >= temp) {
                result = subtract(result, temp);
            }
        }
    }
    return result;
}

// 大数减法
std::string subtract(const std::string& a, const std::string& b) {
    std::string result = a;
    for (int i = b.size() - 1; i >= 0; --i) {
        if (result[i] < b[i]) {
            result[i - 1] -= 1;
            result[i] += 10;
        }
        result[i] -= b[i];
    }
    return result;
}

// 大数比较
bool operator>=(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) {
        return a.size() > b.size();
    }
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return a[i] > b[i];
        }
    }
    return true;
}

// 大数幂运算
std::string power(const std::string& a, const std::string& b, const std::string& n) {
    std::string result = "1";
    for (std::string i = "0"; i < b; i = add(i, "1")) {
        result = mod(multiply(result, a), n);
    }
    return result;
}

// 大数加法
std::string add(const std::string& a, const std::string& b) {
    std::string result(max(a.size(), b.size()) + 1, '0');
    int carry = 0;
    for (int i = 0; i < result.size(); ++i) {
        int temp = carry;
        if (i < a.size()) {
            temp += a[a.size() - 1 - i] - '0';
        }
        if (i < b.size()) {
            temp += b[b.size() - 1 - i] - '0';
        }
        result[result.size() - 1 - i] = temp % 10 + '0';
        carry = temp / 10;
    }
    return result;
}

// 大数比较
bool operator<(const std::string& a, const std::string& b) {
    return !(a >= b);
}

std::string rsa_en(std::string e, std::string message) { //加密，e为公钥，返回加密数据encrypted
    //p和q为两个小素数，e为公钥，d为私钥
    std::string p = "9999991", q = "99999989";
    std::string n = multiply(p, q); //n为p,q的乘积
    std::string padded = pad(message, n.size());//填充
    std::string encrypted = power(padded, e, n);//加密
    return encrypted;
}

std::string rsa_de(std::string d, std::string encrypted) { //解密，d为私钥，返回加密数据unpadded
    //p和q为两个小素数，e为公钥，d为私钥
    std::string p = "9999991", q = "99999989";
    std::string n = multiply(p, q); //n为p,q的乘积
    std::string decrypted = power(encrypted, d, n);//解密
    std::string unpadded = unpad(decrypted);//去填充
    return unpadded;
}

#endif