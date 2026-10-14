// 1.4 补充：cout 怎么知道该打什么
//
// 五样东西，五个盒子，各打一行。注意 double 和 bool 打出来的样子——
// 它们和你心里想的可能不一样。

#include <iostream>
#include <string>

int main() {
    int n = 65;
    double d = 3.0;
    char c = 'A';
    bool b = true;
    std::string s = "A";

    std::cout << n << std::endl;
    std::cout << d << std::endl;
    std::cout << c << std::endl;
    std::cout << b << std::endl;
    std::cout << s << std::endl;

    return 0;
}
