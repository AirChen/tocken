//
//  utils.cpp
//  tocken
//
//  Created by tuRen on 2021/4/6.
//

#include "utils.hpp"
#include <locale>
#include <codecvt>

namespace utils {
    std::wstring s2ws(const std::string& str)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.from_bytes(str);
    }

    std::string ws2s(const std::wstring& wstr)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.to_bytes(wstr);
    }

    //-- 主要用了Unicode(UTF-8)编码的原理分隔字符串
    //-- 简单来说就是每个字符的第一位定义了该字符占据了多少字节
    //-- UTF-8的编码：它是一种变长的编码方式
    //-- 对于单字节的符号，字节的第一位设为0，后面7位为这个符号的unicode码。因此对于英语字母，UTF-8编码和ASCII码是相同的。
    //-- 对于n字节的符号（n>1），第一个字节的前n位都设为1，第n+1位设为0，后面字节的前两位一律设为10。
    //-- 剩下的没有提及的二进制位，全部为这个符号的unicode码。
    size_t get_wchar_shift(std::wstring world, size_t idx) {
        size_t shift = 1;
        wchar_t c = world[idx];
        if (c > 0 && c <= 127) {
            shift = 1;
        } else if (c >= 192 && c <= 223) {
            shift = 2;
        } else if (c >= 224 && c <= 239) {
            shift = 3;
        } else if (c >=240 && c <= 247) {
            shift = 4;
        }
        return shift;
    }
}
