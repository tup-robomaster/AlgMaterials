// 3.2：size() 数的是字节，不是字。
//
// ASCII 那边两个字一样远（一个字节一个字），看不出区别。
// 中文一个字占三个字节（UTF-8 就是这么存的），差一倍就露出来了。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.2-size-utf8.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>

int main() {
    std::string a = "abc";
    std::string b = "小明";
    std::string c = "TUP 算法组";

    std::cout << a.size() << std::endl;
    std::cout << b.size() << std::endl;
    std::cout << c.size() << std::endl;

    return 0;
}
