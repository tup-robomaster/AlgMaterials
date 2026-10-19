// 3.2：两个双引号直接相加，编不过。
//
// "abc" 和 "def" 在编译器眼里不是一个 string，是两块「字面量」，
// 它们之间没有加号这个规矩——得先让一边变成 string。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.2-literal-err.cpp -o /tmp/t

#include <iostream>
#include <string>

int main() {
    std::string s = "abc" + "def";

    std::cout << s << std::endl;

    return 0;
}
