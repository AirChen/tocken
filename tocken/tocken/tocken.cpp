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

//            -- 主要用了Unicode(UTF-8)编码的原理分隔字符串
//            -- 简单来说就是每个字符的第一位定义了该字符占据了多少字节
//            -- UTF-8的编码：它是一种变长的编码方式
//            -- 对于单字节的符号，字节的第一位设为0，后面7位为这个符号的unicode码。因此对于英语字母，UTF-8编码和ASCII码是相同的。
//            -- 对于n字节的符号（n>1），第一个字节的前n位都设为1，第n+1位设为0，后面字节的前两位一律设为10。
//            -- 剩下的没有提及的二进制位，全部为这个符号的unicode码。
static inline size_t get_wchar_shift(std::wstring world, size_t idx) {
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
            std::wcout << world << " " << freq << std::endl;
            _freMap[world] = freq;
            _total_frequence += freq;
            
            for (int i = 0; i < world.size();) {
                size_t shift = get_wchar_shift(world, i);
                std::wstring subWorld(world, 0, i + shift);
                if (_freMap.find(subWorld) == _freMap.end()) {
                    _freMap[subWorld] = 0;
                    
                    std::wcout << subWorld << " 0" << std::endl;
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

void Tocken::get_DAG(std::wstring sentence)
{
    if (check_initialized()) {
        for (int i = 0; i < sentence.size(); i++) {
            std::vector<size_t> endList;

            size_t shift = get_wchar_shift(sentence, i);
            std::wstring tmpKey(sentence, i, shift);
            size_t k = i + shift;
            while (k < sentence.size() && _freMap.find(tmpKey) != _freMap.end()) {
                if (_freMap[tmpKey] > 0) {
                    endList.push_back(k);
                }
                k += get_wchar_shift(sentence, k);
                tmpKey = sentence.substr(i, k - i);
            }
            if (endList.size() == 0) {
                endList.push_back(i);
            }
            _DAG[i] = endList;
        }
    }
}

std::vector<std::wstring> Tocken::_cut_DAG_NO_HMM(std::wstring sentence)
{
    get_DAG(sentence);
    
    int N = (int)sentence.size();
    double logTotal = log((double)_total_frequence);
    _route[N] = std::make_pair(0, 0);
    
    for (int i = N-1; i > -1; i--) {
        double maxLog = -100000;
        size_t maxIdx = -1;
        for (std::vector<size_t>::iterator itr = _DAG[i].begin(); itr != _DAG[i].end(); itr++) {
            double tmpLog = log(std::max((double)_freMap[sentence.substr(i, (*itr + 1 - i))], 1.0)) - logTotal + _route[*itr + 1].first;
            size_t tmpIdx = *itr;
            
            if (tmpLog > maxLog) {
                maxLog = tmpLog;
                maxIdx = tmpIdx;
            }
        }
        _route[i] = std::make_pair(maxLog, maxIdx);
    }
    
    size_t x = 0;
    std::wstring buf = L"";
    std::vector<std::wstring> tocken;
    while (x < N) {
        size_t y = _route[x].second + 1;
        std::wstring subWord = sentence.substr(x, y-x);
        if (subWord.size() == 1 &&
            ((subWord[0] >= 'a' && subWord[0] <= 'z')
             || (subWord[0] >= 'A' && subWord[0] <= 'Z')
             || (subWord[0] >= '0' && subWord[0] <= '9'))) {
            buf += subWord;
        } else {
            if (buf.size() > 0) {
                tocken.push_back(buf);
                buf = L"";
            }
            tocken.push_back(subWord);
        }
        x = y;
    }
    
    return tocken;
}
