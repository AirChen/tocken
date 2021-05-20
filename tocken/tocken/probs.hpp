//
//  probs.hpp
//  tocken
//
//  Created by tuRen on 2021/5/20.
//

#ifndef probs_hpp
#define probs_hpp

#include <map>

using std::map;

extern map<char, double> begin_p;
extern map<char, map<char, double>> trans_p;
extern map<char, map<wchar_t, double>> emit_p;

#endif /* probs_hpp */
