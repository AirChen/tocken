//
//  tocken.hpp
//  tocken
//
//  Created by airChen on 2021/3/28.
//

#ifndef tocken_hpp
#define tocken_hpp

#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::unique_ptr;
using std::unordered_map;

class Token {
public:
    Token() {}
    virtual ~Token() {}

    virtual bool cut(const string& sentence, vector<string>& res, bool useHMM = false) = 0;
    virtual bool readFile(const char* fname) = 0;
};

class TokenImp: public Token {
public:
    TokenImp() {}
    ~TokenImp() {}
    
    virtual bool cut(const string& sentence, vector<string>& res, bool useHMM = false) override;
    virtual bool readFile(const char* fname) override;

private:
    void get_DAG(const string& sentence);
    bool _cut_DAG_NO_HMM(const string& sentence, vector<string>& res);
    bool _cut_DAG(const string& sentence, vector<string>& res);
    
    size_t _total_frequence;
    unordered_map<string, size_t> _freMap;
    unordered_map<size_t, std::vector<size_t>> _DAG;
    unordered_map<size_t, std::pair<double, size_t>> _route;
};

#endif /* tocken_hpp */
