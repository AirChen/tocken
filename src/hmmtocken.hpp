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

using std::string;
using std::vector;

class HMM {
    static vector<string> _cut(string sentence);
    static vector<string> _segHan(string sentence);
    static vector<string> _segSig(string sentence);
public:
    static vector<string> cut(string sentence);
    static void add_force_splite(string splite);
        
};

#endif /* hmmtocken_hpp */
