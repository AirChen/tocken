//
//  main.cpp
//  tocken
//
//  Created by AirChen on 2021/3/28.
//

#include <iostream>
#include "tocken.hpp"

int main(int argc, const char * argv[]) {    
    Tocken tocken;
    std::vector<std::string> res = tocken._cut_DAG_NO_HMM("今天恭喜发送财红包拿来");
    for (auto w : res) {
        std::cout << w << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
