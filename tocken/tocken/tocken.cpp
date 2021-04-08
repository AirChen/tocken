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
#include "utils.hpp"

void Tocken::loadPreFrequenceDict(std::string path) {
    std::ifstream fs(path);
    _total_frequence = 0;

    if (fs.is_open()) {
        std::string line;
        while(std::getline(fs, line))
        {
            std::string world;
            std::string freqW;

            std::istringstream lineStream(line);
            lineStream >> world;
            lineStream >> freqW;

            size_t freq = std::stoi(freqW);
            std::wstring key = utils::s2ws(world);
            _freMap[key] = freq;
            _total_frequence += freq;

            for (int i = 0; i < key.size(); i++) {
                std::wstring subWorld(key, 0, i+1);
                if (_freMap.find(subWorld) == _freMap.end()) {
                    _freMap[subWorld] = 0;
                }
            }
        }
        fs.close();
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
            size_t k = i;
            std::wstring tmpKey(sentence, i, 1);
            while (k <= sentence.size() && _freMap.find(tmpKey) != _freMap.end()) {
                if (_freMap[tmpKey] > 0) {
                    endList.push_back(k);
                }
                k++;
                tmpKey = sentence.substr(i, k + 1 - i);
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
