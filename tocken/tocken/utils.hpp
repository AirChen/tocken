//
//  utils.hpp
//  tocken
//
//  Created by tuRen on 2021/4/6.
//

#ifndef utils_hpp
#define utils_hpp

#include <stdio.h>
#include <string>

namespace utils {
    size_t get_wchar_shift(std::wstring world, size_t idx);
    std::wstring s2ws(const std::string& str);
    std::string ws2s(const std::wstring& wstr);
}

#endif /* utils_hpp */
