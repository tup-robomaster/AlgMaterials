// 3.1：范围 for——不数下标的那种写遍历的法子。
//
// 三段代码，只差一个 &。没有 & 的那一段修改的是副本，v 保持不变；
// 有 & 的那一段修改的才是 v 中的元素——基础课 6.2 学的引用，在这里再次出现。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.1-range-for.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {90, 85, 77};

    for (int x : v) {
        std::cout << x << std::endl;
    }

    std::cout << "--- 没有 & 的循环 ---" << std::endl;

    for (int x : v) {
        x = x + 1000;
    }

    for (int x : v) {
        std::cout << x << std::endl;
    }

    std::cout << "--- 有 & 的循环 ---" << std::endl;

    for (int& x : v) {
        x = x + 1;
    }

    for (int x : v) {
        std::cout << x << std::endl;
    }

    return 0;
}
