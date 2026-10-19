// 4.2：迭代器真正的用处——「记住一个位置」。
//
// 找一个及格的：没找到就向后移动一个位置，找到就停。停下来的时候 it 指向那个位置。
// 走完了还没找到，it 正好等于 end()——所以 end() 是个「没找到」的记号。
//
// 最后那行 it - score.begin() 是「它离起点有几格」，也就是它的编号。
// 减法只对 vector 这种能一步跨几格的迭代器成立（map 的不行，4.2 末尾讲）。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.2-iter-find.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<int> score = {45, 52, 68, 91, 73};

    auto it = score.begin();
    while (it != score.end() && *it < 60) {
        ++it;
    }

    if (it == score.end()) {
        std::cout << "一个及格的都没有" << std::endl;
    } else {
        std::cout << "第一个及格的是 " << *it << std::endl;
        std::cout << "它在第 " << (it - score.begin()) << " 个" << std::endl;
    }

    return 0;
}
