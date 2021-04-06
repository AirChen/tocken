//
//  tocken.cpp
//  tocken
//
//  Created by 志哥哥 on 2021/3/28.
//

#include "tocken.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

void Tocken::loadPreFrequenceDict(std::string path) {
    std::wifstream wfs(path);
    _total_frequence = 0;
    
    if (wfs.is_open()) {
        std::wstring line;
        while(std::getline(wfs, line))
        {
            std::wstring world;
            std::wstring freqW;
            
            std::wistringstream lineStream(line);
            lineStream >> world;
            lineStream >> freqW;
            
            size_t freq = std::stoi(freqW);
            
            std::string key(world.begin(), world.end());
            std::cout << key << " " << freq << std::endl;
            _freMap[key] = freq;
            _total_frequence += freq;
            
//            -- 主要用了Unicode(UTF-8)编码的原理分隔字符串
//            -- 简单来说就是每个字符的第一位定义了该字符占据了多少字节
//            -- UTF-8的编码：它是一种变长的编码方式
//            -- 对于单字节的符号，字节的第一位设为0，后面7位为这个符号的unicode码。因此对于英语字母，UTF-8编码和ASCII码是相同的。
//            -- 对于n字节的符号（n>1），第一个字节的前n位都设为1，第n+1位设为0，后面字节的前两位一律设为10。
//            -- 剩下的没有提及的二进制位，全部为这个符号的unicode码。
            for (int i = 0; i < world.size();) {
                size_t shift = 1;
                wchar_t c = world[i];
                if (c > 0 && c <= 127) {
                    shift = 1;
                } else if (c >= 192 && c <= 223) {
                    shift = 2;
                } else if (c >= 224 && c <= 239) {
                    shift = 3;
                } else if (c >=240 && c <= 247) {
                    shift = 4;
                }
                
                std::string subWorld(key, 0, i + shift);
                if (_freMap.find(subWorld) == _freMap.end()) {
                    _freMap[subWorld] = 0;
                    
                    std::cout << subWorld << " 0" << std::endl;
                }
                
                i += shift;
            }
        }
        wfs.close();
        _initialized = true;
    }
};

bool Tocken::check_initialized()
{
    if (!_initialized) {
        loadPreFrequenceDict("source/dict.txt");
    }
    return _initialized;
}

void Tocken::get_DAG(std::string sentence)
{
    if (check_initialized()) {
        for (int i = 0; i < sentence.size(); i++) {
            std::vector<int> endList;
            int k = i + 1;
            std::string tmpKey(sentence, k, 1);
            while (k < sentence.size() && _freMap.find(tmpKey) != _freMap.end()) {
                if (_freMap[tmpKey] > 0) {
                    endList.push_back(k);
                }
                k++;
                tmpKey = sentence.substr(i, k - i);
            }
            if (endList.size() == 0) {
                endList.push_back(i);
            }
            _DAG[i] = endList;
        }
    }
}
