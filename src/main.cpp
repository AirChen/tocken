//
//  main.cpp
//  tocken
//
//  Created by AirChen on 2021/3/28.
//

#include <iostream>
#include "tocken.hpp"
#include "utils.hpp"
#include <locale>
#include <codecvt>
#include <chrono>
using std::chrono::duration_cast; 
using std::chrono::milliseconds; 
using std::chrono::system_clock;

int main(int argc, const char * argv[]) {
    if (argc < 2) {
        std::cout << "cli exp: ./token fname" << std::endl;
        return -1;
    }
    const char* fname = argv[1];
    
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    
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
            wstring inputWstr = converterX.from_bytes(inputStr);
            std::vector<std::wstring> res = token->cut(inputWstr);
            std::cout << "result: " << std::endl;
            for (auto w : res) {
                if (w.size() > 0) {
                    std::cout << utils::ws2s(w) << "  ";
                }
            }
            std::cout << std::endl;
        }
    }
    
    return 0;
}
