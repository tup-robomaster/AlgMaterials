// 4.5：方括号里漏写了——这一份是故意无法编译的。
//
// 外面那个 line 忘了写进方括号，lambda 里就当作不存在，
// 编译器说的正是这几个字：not captured（没有被捕获）。
// 跟着的几句 note：一句说这个 lambda 什么也没有捕获，一句指出 line 是在哪里声明的。
//
// 看报错：g++ -std=c++17 -Wall 示例代码/4.5-capture-err.cpp -o /tmp/t

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> score = {45, 52, 68, 91, 73};

    int line = 60;
    auto it = std::find_if(score.begin(), score.end(),
                           [](int x) { return x >= line; });

    std::cout << *it << std::endl;

    return 0;
}
