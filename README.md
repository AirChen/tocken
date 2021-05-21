# tocken

Chinese token demo, reference [jieba repo](https://github.com/fxsjy/jieba).

```
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
```

then output:

```
result: 
这是 一个 伸手不见五指 的 黑夜 。 我 叫 孙悟空 ， 我 爱 北京 ， Python 我 爱 和 C++ 。 
```

At branch [server](https://github.com/AirChen/token/tree/server), we integrate [workflow](https://github.com/sogou/workflow) lib created by Sogou Inc. 
and this is a http server demo, deploy to work space.

Having Enjoy it!
