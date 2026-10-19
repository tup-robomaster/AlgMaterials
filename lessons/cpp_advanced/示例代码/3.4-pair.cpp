// 3.4：pair——两样东西捆一起，一个叫 first，一个叫 second。
//
// 它是最小的容器：不多不少正好两个。地图上「名字 -> 数」的每一项
// 就是这个形状，所以先认得它，map 才看得懂。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.4-pair.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>
#include <utility>

int main() {
    std::pair<std::string, int> p("apple", 3);

    std::cout << p.first << std::endl;
    std::cout << p.second << std::endl;

    p.second = p.second + 1;          // 两个都能改
    p.first = "pear";
    std::cout << p.first << " " << p.second << std::endl;

    std::pair<std::string, int> q = {"banana", 5};   // 另一种写法，一样的东西
    std::cout << q.first << " " << q.second << std::endl;

    return 0;
}
