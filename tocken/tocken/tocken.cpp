//
//  tocken.cpp
//  tocken
//
//  Created by 志哥哥 on 2021/3/28.
//

#include "tocken.hpp"
#include <fstream>
#include <iostream>

void Tocken::loadPreFrequenceDict(std::string path) {
    std::ifstream fs(path);
    _total_frequence = 0;
    if (fs.is_open()) {
        std::string line;
        while(std::getline(fs, line))
        {
            char* tok = strtok((char*)line.c_str(), " ");
            char* tok1 = strtok(NULL, " ");
//                std::cout << tok << " " << tok1 << std::endl;
            
            int req = 0;
            while (*tok1) {
                req = *tok1 - '0' + req * 10;
                tok1++;
            }
            std::cout << tok << " " << req << std::endl;
            
            _freMap[tok] = req;
            _total_frequence += req;
            
            for (int i = 1; i < strlen(tok); i++) {
                std::string subWorld(tok, i);
                subWorld[i+1] = 0;
                if (_freMap.find(subWorld) == _freMap.end()) {
                    _freMap[subWorld] = 0;
                    
                    std::cout << subWorld << " 0" << std::endl; // unicode code miss !!!
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
        loadPreFrequenceDict("source/test.txt");
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

std::vector<std::string> Tocken::_cut_DAG_NO_HMM(std::string sentence)
{
    get_DAG(sentence);
    
    int N = (int)sentence.size();
    double logTotal = log((double)_total_frequence);
    _route[N] = std::make_pair(0, 0);
    
    for (int i = N-1; i > -1; i--) {
        double maxLog = -100000;
        size_t maxIdx = -1;
        for (std::vector<int>::iterator itr = _DAG[i].begin(); itr != _DAG[i].end(); itr++) {
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
    std::string buf = "";
    std::vector<std::string> tocken;
    while (x < N) {
        size_t y = _route[x].second + 1;
        std::string subWord = sentence.substr(x, y-x);
        if (subWord.size() == 1 &&
            ((subWord[0] >= 'a' && subWord[0] <= 'z')
             || (subWord[0] >= 'A' && subWord[0] <= 'Z')
             || (subWord[0] >= '0' && subWord[0] <= '9'))) {
            buf += subWord;
        } else {
            if (buf.size() > 0) {
                tocken.push_back(buf);
                buf = "";
            }
            tocken.push_back(subWord);
        }
        x = y;
    }
    
    return tocken;
}
