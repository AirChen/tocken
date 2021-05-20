//
//  main.cpp
//  tocken
//
//  Created by AirChen on 2021/3/28.
//

#include <iostream>
#include "tocken.hpp"
#include "utils.hpp"
#include "hmmtocken.hpp"

int main(int argc, const char * argv[]) {    
//    Tocken tocken;
//
//    std::vector<std::wstring> res = tocken._cut_DAG_NO_HMM(L"今天恭喜发送财红包拿来");
//
//    std::cout << "result: " << std::endl;
//    for (auto w : res) {
//        std::cout << utils::ws2s(w) << " ";
//    }
//    std::cout << std::endl;
    
    for (auto tok : HMM::cut(L"永和服装饰品有限公司")) {
        std::cout << utils::ws2s(tok) << " ";
    }
    std::cout << std::endl;
        
    return 0;
}
