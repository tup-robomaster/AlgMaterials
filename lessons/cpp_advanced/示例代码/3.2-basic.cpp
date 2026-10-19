// 3.2：string 不只是「装一串字」——它能接、能比、能按下标取。
//
// 基础课 1.4 说了 char 和 string 的区别，可 string 本身一直只用它装东西。
// 这一份把它当个东西看：它有自己的办法。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.2-basic.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>

int main() {
    std::string first = "TUP";
    std::string last = "Algorithm";

    std::string whole = first + " " + last;      // 接起来
    std::cout << whole << std::endl;

    std::string s = "abc";
    s += "de";                                   // 后面接着长
    std::cout << s << std::endl;

    std::cout << s.size() << std::endl;          // 几个字符
    std::cout << s[0] << std::endl;              // 第一个
    std::cout << s[int(s.size()) - 1] << std::endl;  // 最后一个

    std::string name = "小明";
    std::cout << "你好，" + name << std::endl;   // 左边是双引号，右边是 string

    if (s == "abcde") {
        std::cout << "s 就是 abcde" << std::endl;
    }
    if (s < "abcz") {
        std::cout << "abcde 排在 abcz 前面" << std::endl;
    }

    return 0;
}
