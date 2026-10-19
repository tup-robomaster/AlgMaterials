// 4.2：迭代器——「指着某一个位置」的那个东西。
//
// 三格，从头走到尾。四句话就是它的全部：
//   begin() 是第一格；end() 是最后一格的后面那一格（不是最后一格）；
//   ++ 是逐个位置向后移动；*it 是「它指向的那个位置上的元素」。
// 动画《迭代器逐个位置移动》的每一步，跟这份程序的输出完全相同。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.2-iter-walk.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {3, 1, 4};

    auto it = v.begin();
    std::cout << *it << std::endl;

    ++it;
    std::cout << *it << std::endl;

    ++it;
    std::cout << *it << std::endl;

    ++it;
    if (it == v.end()) {
        std::cout << "已到达末尾" << std::endl;
    }

    return 0;
}
