// 3.1：嵌套 vector——一张三行三列的表。
// 外层装行，每一行又是一个 vector，装这一行里的数。
// 取一个数用两个下标：grid[1][2] 是第二行第三个数（下标都从 0 数起）。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.1-nested.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<std::vector<int>> grid = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    std::cout << grid.size() << std::endl;       // 几行
    std::cout << grid[1].size() << std::endl;   // 第二行几个数
    std::cout << grid[1][2] << std::endl;       // 第二行第三个数

    for (std::vector<int>& row : grid) {
        for (int x : row) {
            std::cout << x << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
