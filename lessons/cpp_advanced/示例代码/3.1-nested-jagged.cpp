// 3.1：嵌套 vector 的每一行可以不一样长。
// 每一行是一个独立的 vector，自己长自己的：push_back 进几个就是几个。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.1-nested-jagged.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<int>> groups;

    groups.push_back(std::vector<int>{90, 85});
    groups.push_back(std::vector<int>{77, 88, 95, 60});

    for (std::vector<int>& g : groups) {
        std::cout << g.size() << ": ";
        for (int x : g) {
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
