// 练习 3-11 的答案。pair 的两个成员分别叫 first 和 second，
// 名字固定，跟里面装的是什么没关系。

#include <iostream>
#include <string>
#include <utility>

int main() {
    std::pair<std::string, int> p = {"小明", 90};

    std::cout << p.first << " " << p.second << std::endl;

    p.second = p.second + 5;

    std::cout << p.first << " " << p.second << std::endl;

    return 0;
}
