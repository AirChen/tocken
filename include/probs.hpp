//
//  probs.hpp
//  tocken
//
//  Created by tuRen on 2021/5/20.
//

#ifndef probs_hpp
#define probs_hpp

#include <unordered_map>

using std::unordered_map;

extern unordered_map<char, double> begin_p;
extern unordered_map<char, unordered_map<char, double>> trans_p;
extern unordered_map<char, unordered_map<wchar_t, double>> emit_p;

#endif /* probs_hpp */
