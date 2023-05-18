//
//  hmmtocken.cpp
//  tocken
//
//  Created by tuRen on 2021/5/20.
//

#include "hmmtocken.hpp"
#include "probs.hpp"
#include <float.h>
#include <regex>
#include <iostream>

#define DOUBLE_MIN -3.14e100

extern unordered_map<char, double> begin_p;
extern unordered_map<char, unordered_map<char, double>> trans_p;
extern unordered_map<char, unordered_map<wchar_t, double>> emit_p;

using std::string;
static unordered_map<char, string> _PrevStatus = {
    {'B', "ES"},
    {'M', "MB"},
    {'S', "SE"},
    {'E', "BM"}
};

static vector<string> _Force_Split_Words;

static void viterbi(string sentence, double& prob, string& problist) {
    string states("BMES");
    unordered_map<int, unordered_map<char, double>> V;
    unordered_map<char, string> path;
    
    for (auto y : states) {
        auto itr = emit_p[y].find(sentence[0]);
        double em_p = ((itr == emit_p[y].end()) ? DOUBLE_MIN : (*itr).second);
        V[0][y] = begin_p[y] + em_p;
        path[y].push_back(y);
    }
    
    for (int i = 1; i < sentence.size(); i++) {
        unordered_map<char, string> tmpPath;
        for (auto y : states) {
            auto itr = emit_p[y].find(sentence[i]);
            double em_p = ((itr == emit_p[y].end()) ? DOUBLE_MIN : (*itr).second);
            
            double maxProb = DOUBLE_MIN;
            char maxPreState = _PrevStatus[y][0];
            for (auto pre : _PrevStatus[y]) {
                auto itr0 = trans_p[pre].find(y);
                double ts_p = ((itr0 == trans_p[pre].end()) ? DOUBLE_MIN : (*itr0).second);
                
                double prob = V[i-1][pre] + ts_p + em_p;
                if (prob > maxProb) {
                    maxProb = prob;
                    maxPreState = pre;
                }
            }
            V[i][y] = maxProb;
            tmpPath[y] = path[maxPreState] + y;
        }
        path = tmpPath;
    }
    
    double maxProb = DOUBLE_MIN;
    char maxState = ' ';
    string endStr = "ES";
    for (auto y : endStr) {
        double prob = V[(int)sentence.size()-1][y];
        if (prob > maxProb) {
            maxProb = prob;
            maxState = y;
        }
    }
    
    prob = maxProb;
    problist = path[maxState];
}

vector<string> HMM::_cut(string sentence) {
    double prob;
    string problist;
    viterbi(sentence, prob, problist);
    
    vector<string> ans;
    int begin = 0, nexti = 0;
    for (int i = 0; i < sentence.size(); i++) {
        char pos = problist[i];
        switch (pos) {
            case 'B':
            {
                begin = i;
            }
                break;
            case 'E':
            {
                string str = sentence.substr(begin, i+1-begin);
                ans.push_back(str);
                
                nexti = i+1;
            }
                break;
            case 'S':
            {
                string str;
                str.push_back(sentence[i]);
                ans.push_back(str);
                
                nexti = i+1;
            }
                
            default:
                break;
        }
    }
    
    if (nexti < sentence.size()) {
        string str = sentence.substr(nexti, sentence.size()-nexti-1);
        ans.push_back(str);
    }
    
    return ans;
}

vector<string> HMM::cut(string sentence) {
    vector<string> ans;
    vector<string> segs = _segHan(sentence);
    
    std::regex reg("([\u4E00-\u9FD5]+)");
    std::smatch sm;
    for (auto wstr : segs) {
        if (std::regex_match(wstr.cbegin(), wstr.cend(), sm, reg)) {
            if (sm.size() == 2) {
                for (auto wsubstr : _cut(wstr)) {
                    if (std::find(_Force_Split_Words.begin(), _Force_Split_Words.end(), wsubstr) == _Force_Split_Words.end()) {
                        ans.push_back(wsubstr);
                    } else {
                        for (auto c : wsubstr) {
                            string s;
                            s.push_back(c);
                            ans.push_back(s);
                        }
                    }
                }
            } else {
                printf("error: matched faild.\n");
            }
        } else {
            vector<string> sigs = _segSig(wstr);
            ans.insert(ans.end(), sigs.begin(), sigs.end());
        }
    }
    
    return ans;
}

using std::regex;
vector<string> _wsegRegex(string& sentence, regex& regex) {
    vector<string> v(std::sregex_token_iterator(sentence.begin(), sentence.end(), regex, -1), std::sregex_token_iterator());
    std::smatch sm;
    string::const_iterator itr = sentence.cbegin();
    while (std::regex_search(itr, sentence.cend(), sm, regex)) {
        itr = sm[0].second;
                
        if (sm.size() == 2) {
            string s1 = sm[0].str();
            v.push_back(s1);
        }
    }
    
    return v;
}

vector<string> HMM::_segHan(string sentence) {
    std::regex reg("([\u4E00-\u9FD5]+)");
    return _wsegRegex(sentence, reg);
}

vector<string> HMM::_segSig(string sentence) {
    std::regex reg("([a-zA-Z0-9]+(?:\.\d+)?%?)");
    return _wsegRegex(sentence, reg);
}

void HMM::add_force_splite(string splite) {
    _Force_Split_Words.push_back(splite);
}
