// 4.5：统计有几个合格的——count 无法完成这个任务。
//
// std::count 只统计「等于几」的元素，条件形式它不接受。
// 要统计「满足一个条件的有几个」，使用 count_if，条件还是那条 lambda。
// 名字上 if / _if 的区别就是全部区别：带 _if 的接受条件。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.5-count-if.cpp -o /tmp/t && /tmp/t

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> score = {45, 52, 68, 91, 73};

    std::cout << "等于 52 的有 "
              << std::count(score.begin(), score.end(), 52) << " 个" << std::endl;

    int pass = std::count_if(score.begin(), score.end(),
                             [](int x) { return x >= 60; });
    std::cout << "及格的（>= 60）有 " << pass << " 个" << std::endl;

    return 0;
}
