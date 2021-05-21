//
//  main.cpp
//  tocken
//
//  Created by AirChen on 2021/3/28.
//

#include <iostream>
#include "tocken.hpp"
#include "utils.hpp"

int main(int argc, const char * argv[]) {
    auto token = Token::instance();
    std::vector<std::wstring> res = token->cut(L"这是一个伸手不见五指的黑夜。我叫孙悟空，我爱北京，我爱Python和C++。");
    std::cout << "result: " << std::endl;
    for (auto w : res) {
        if (w.size() > 0) {            
            std::cout << utils::ws2s(w) << " ";
        }
    }
    std::cout << std::endl;
        
    return 0;
}
