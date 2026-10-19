// 3.2：那两条字面量要接起来，得这么写。三条路，都真跑过。
//
// 跟 3.2-literal-err.cpp 是一对：那一份编不过，这一份编得过。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.2-literal-ok.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>

int main() {
    // 路子一：让左边先变成 string
    std::string a = std::string("abc") + "def";
    std::cout << a << std::endl;

    // 路子二：中间夹一个已经有的 string
    std::string mid = "abc";
    std::string b = mid + "def";
    std::cout << b << std::endl;

    // 路子三：两段字面量写在一起，编译器会自动接起来——中间连加号都没有
    std::string c = "abc" "def";
    std::cout << c << std::endl;

    return 0;
}
