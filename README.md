# token

Chinese sentence token lib, reference [jieba repo](https://github.com/fxsjy/jieba).

## Compile

Cloning the git repo
```
git clone https://github.com/AirChen/token.git
```

Then compile the lib
```
cd token

mkdir build

cd build
cmake ..
make -j8
```

## Cli Demo:

```
unique_ptr<Token> token = std::make_unique<TokenImp>();
if (token->readFile(fname)) {
    char cliStr[1024];
    while (true) {
        std::cout << "Please input your sentence: " << std::endl;
        scanf("%s", cliStr);

        std::string inputStr{cliStr};
        std::vector<std::string> res;
        if (token->cut(inputStr, res)) {
            for (auto w : res) {
                std::cout << w << "  ";
            }
        }
        std::cout << std::endl;
    }
}
```

the output:

```
result: 
这是 一个 伸手不见五指 的 黑夜 。 我 叫 孙悟空 ， 我 爱 北京 ， Python 我 爱 和 C++ 。 
```

Having Enjoy it!
