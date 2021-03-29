//
//  tocken.hpp
//  tocken
//
//  Created by 志哥哥 on 2021/3/28.
//

#ifndef tocken_hpp
#define tocken_hpp

#include <stdio.h>
#include <map>
#include <vector>

struct Tocken {
    void loadPreFrequenceDict(std::string path);
    bool check_initialized();
    void get_DAG(std::string sentence);
    
    std::vector<std::string> _cut_DAG_NO_HMM(std::string sentence);
    
    Tocken() { _initialized = false; };
    ~Tocken() {};
    
private:
    size_t _total_frequence;
    std::map<std::string, size_t> _freMap;
    std::map<size_t, std::vector<int>> _DAG;
    std::map<size_t, std::pair<double, size_t>> _route;
    
    bool _initialized;
};

#endif /* tocken_hpp */
