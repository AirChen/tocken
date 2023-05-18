//
//  main.cpp
//  tocken
//
//  Created by AirChen on 2021/3/28.
//

#include <iostream>
#include "tocken.hpp"
#include "utils.hpp"
#include <chrono>
using std::chrono::duration_cast; 
using std::chrono::milliseconds; 
using std::chrono::system_clock;

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        std::cout << "cli exp: ./token fname useHMM" << std::endl;
        return -1;
    }
    const char* fname = argv[1];
    const char* useHMM = argv[2];
    bool isUseHMM = false;
    if (strcmp(useHMM, "use") == 0) {
        isUseHMM = true;
    }
    
    auto t0 = system_clock::now();
    unique_ptr<Token> token = std::make_unique<TokenImp>();
    if (token->readFile(fname)) {
        auto t1 = system_clock::now();
        auto useTime = duration_cast<milliseconds>(t1 - t0);
        std::cout << "Load dictionary success! "
            "Use Time: " << useTime.count() << " (ms)" << std::endl;
        
        char cliStr[1024];
        while (true) {
            std::cout << "Please input your sentence: " << std::endl;
            scanf("%s", cliStr);

            std::string inputStr{cliStr};
            std::vector<std::string> res;
            if (token->cut(inputStr, res, isUseHMM)) {
                std::cout << "result: " << std::endl;
                for (auto w : res) {
                    if (w.size() > 0) {
                        std::cout << w << "  ";
                    }
                }
            }
            std::cout << std::endl;
        }
    }
    
    return 0;
}
