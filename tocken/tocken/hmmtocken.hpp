//
//  hmmtocken.hpp
//  tocken
//
//  Created by tuRen on 2021/5/20.
//

#ifndef hmmtocken_hpp
#define hmmtocken_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <map>

using std::wstring;
using std::vector;
using std::map;

class HMM {
    static vector<wstring> _cut(wstring sentence);
    static vector<wstring> _segHan(wstring sentence);
    static vector<wstring> _segSig(wstring sentence);
public:
    static vector<wstring> cut(wstring sentence);
    static void add_force_splite(wstring splite);
        
};

#endif /* hmmtocken_hpp */
