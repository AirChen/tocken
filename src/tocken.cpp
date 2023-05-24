//
//  tocken.cpp
//  tocken
//
//  Created by airChen on 2021/3/28.
//

#include "tocken.hpp"
#include "hmmtocken.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

using std::string_view;

void split(const string& str, vector<string>& res, char ch) {
    char chs[512];
    strcpy(chs, str.c_str());
    char s[2] = " ";
    s[0] = ch;

    char* token = strtok(chs, s);
    while (token != NULL) {
        std::string tokStr{ token };
        res.emplace_back(tokStr);

        token = strtok(NULL, s);
    }
}

bool TokenImp::readFile(const char* fname) {
    std::ifstream ifsr(fname);
    if (!ifsr.is_open()) {
        std::cout << "Err in open file: " << fname << std::endl;
        return false;
    }

    _total_frequence = 0;
    
    std::string line;
    while (std::getline(ifsr, line)) {
        vector<string> toks;
        split(line, toks, ' ');
        if (toks.size() < 2) {
            continue;
        }
        string key = toks[0];
        string freqs = toks[1];
        
        size_t freq = std::stoi(freqs);
        _freMap[key] = freq;
        _total_frequence += freq;

        /* string_view keyView{key}; */
        size_t len = key.size();
        for (size_t i = 0; i < len; i++) {
            string subWorld = key.substr(0, i + 1);
            if (_freMap.find(subWorld) == _freMap.end()) {
                _freMap[subWorld] = 0;
            }
        }

    }
    ifsr.close();

    return true;
}

void TokenImp::get_DAG(const string& sentence)
{
    for (int i = 0; i < sentence.size(); i++) {
        vector<size_t> endList;
        size_t k = i;
        string tmpKey(sentence, i, 1);
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

bool TokenImp::_cut_DAG(const string& sentence, vector<string>& res)
{
    get_DAG(sentence);
    
    int N = (int)sentence.size();
    double logTotal = log((double)_total_frequence);
    _route[N] = std::make_pair(0, 0);
    
    for (int i = N-1; i > -1; i--) {
        double maxLog = -100000;
        size_t maxIdx = -1;
        for (vector<size_t>::iterator itr = _DAG[i].begin(); itr != _DAG[i].end(); itr++) {
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
    string buf;
    
    auto generate = [&](){
        if (buf.size() == 1) {
            res.push_back(buf);
        } else if (_freMap.find(buf) == _freMap.end()) {
            vector<string> recognized;
            HMM::cut(buf, recognized);
            res.insert(res.end(), recognized.begin(), recognized.end());
        } else {
            for (auto c : buf) {
                string str{c};
                
                res.push_back(str);
            }
        }
    };
    
    while (x < N) {
        size_t y = _route[x].second + 1;
        string subWord = sentence.substr(x, y-x);
        if (subWord.size() == 1) {
            buf += subWord;
        } else {
            if (buf.size() > 0) {
                generate();
                buf.clear();
            }
            res.push_back(subWord);
        }
        x = y;
    }
    
    if (buf.size() > 0) {
        generate();
    }
    
    return true;
}

bool TokenImp::_cut_DAG_NO_HMM(const string& sentence, vector<string>& res)
{
    get_DAG(sentence);
    
    int N = (int)sentence.size();
    double logTotal = log((double)_total_frequence);
    _route[N] = std::make_pair(0, 0);
    
    for (int i = N-1; i > -1; i--) {
        double maxLog = -100000;
        size_t maxIdx = -1;
        for (vector<size_t>::iterator itr = _DAG[i].begin(); itr != _DAG[i].end(); itr++) {
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
    string buf;
    std::regex reg("([a-zA-Z0-9]+(?:\.\d+)?%?)");
    std::smatch sm;
    
    while (x < N) {
        size_t y = _route[x].second + 1;
        string subWord = sentence.substr(x, y-x);
        if (subWord.size() == 1 && std::regex_match(subWord.cbegin(), subWord.cend(), sm, reg)) {
            if (sm.size() == 2) {
                buf += subWord;
            } else {
                printf("error: matched faild.\n");
            }
        } else {
            if (buf.size() > 0) {
                res.push_back(buf);
                buf.clear();
            }
            res.push_back(subWord);
        }
        x = y;
    }
    
    return true;
}

bool TokenImp::cut(const string& sentence, vector<string>& res, bool useHMM) {
    return useHMM ? _cut_DAG(sentence, res) : _cut_DAG_NO_HMM(sentence, res);
}

