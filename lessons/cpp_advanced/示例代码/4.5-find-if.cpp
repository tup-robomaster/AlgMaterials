// 4.5：find 只判断「等于几」，find_if 接受的是你自己给定的一条条件。
//
// 条件是一小段代码，直接写在调用的括号里——这就是 lambda。
// 方括号开头，圆括号里是它的参数（这儿是容器里的一个元素），
// 花括号里是「满足什么条件」，返回 true / false。
// 那对中括号是 lambda 的记号，跟下标、跟数组都没关系。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.5-find-if.cpp -o /tmp/t && /tmp/t

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> score = {45, 52, 68, 91, 73};

    auto it = std::find_if(score.begin(), score.end(),
                           [](int x) { return x >= 60; });

    if (it == score.end()) {
        std::cout << "一个及格的都没有" << std::endl;
    } else {
        std::cout << "第一个及格的是 " << *it << std::endl;
    }

    return 0;
}
