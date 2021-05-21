//
//  tocken.hpp
//  tocken
//
//  Created by airChen on 2021/3/28.
//

#ifndef tocken_hpp
#define tocken_hpp

#include <stdio.h>
#include <map>
#include <vector>
#include <string>

using std::string;
using std::wstring;
using std::vector;
using std::unique_ptr;

class Token {
public:
    virtual vector<wstring> cut(wstring sentence, bool useHMM = true) = 0;
    
    static unique_ptr<Token> instance();
    virtual ~Token() {};
};

class TokenImp: public Token {
    void loadPreFrequenceDict(string path);
    bool check_initialized();
    void get_DAG(wstring sentence);
    
    vector<wstring> _cut_DAG_NO_HMM(wstring sentence);
    vector<wstring> _cut_DAG(wstring sentence);
    
    size_t _total_frequence;
    std::map<wstring, size_t> _freMap;
    std::map<size_t, std::vector<size_t>> _DAG;
    std::map<size_t, std::pair<double, size_t>> _route;
    
    bool _initialized;
    
public:
    virtual vector<wstring> cut(wstring sentence, bool useHMM);
    
    using Token::Token;
    TokenImp(): _initialized(false) {};
    ~TokenImp() {};
};

#endif /* tocken_hpp */
