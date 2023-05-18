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
public:
    static bool cut(const string& sentence, vector<string>& res);
    static void add_force_splite(const string& splite);
private:
    static bool _cut(const string& sentence, vector<string>& res);
    static bool _segHan(const string& sentence, vector<string>& res);
    static bool _segSig(const string& sentence, vector<string>& res);
};

#endif /* hmmtocken_hpp */
