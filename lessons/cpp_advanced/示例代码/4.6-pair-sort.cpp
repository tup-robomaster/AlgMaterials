// 4.6：pair 不用给规矩——它本身就会比较。
//
// 规矩是现成的：先比 first，first 一样再比 second。
// 所以 pair 的 vector 直接 sort 就行，一行 lambda 都不用写。
// 3.4 里 map 输出时自己是有序的，用的就是这条——
// map 里装的正是一堆 pair。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.6-pair-sort.cpp -o /tmp/t && /tmp/t

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::vector<std::pair<std::string, int>> v = {{"小李", 90}, {"小王", 77},
                                                  {"小李", 60}};

    std::sort(v.begin(), v.end());

    for (const std::pair<std::string, int>& p : v) {
        std::cout << p.first << " " << p.second << std::endl;
    }

    return 0;
}
