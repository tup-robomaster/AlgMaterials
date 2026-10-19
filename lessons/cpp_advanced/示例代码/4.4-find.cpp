// 4.4：<algorithm> 里那些现成的算法——刚学的迭代器就是它们的进出料口。
//
// 它们一律需要两项内容：从哪里开始、到哪里为止（两个迭代器），
// 外加要查找的元素。找到就返回指向该位置的迭代器，找不到返回第二个——
// 也就是 end()。所以「找到没找到」始终是用结果与 v.end() 比较。
//
// 一个容易混的地方：string 自己也有个 find（3.2 学的），
// 那个返回的是编号；这里的 std::find 返回的是迭代器。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.4-find.cpp -o /tmp/t && /tmp/t

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {45, 52, 68, 91, 73};

    auto it = std::find(v.begin(), v.end(), 68);
    if (it == v.end()) {
        std::cout << "没找到 68" << std::endl;
    } else {
        std::cout << "找到 68，它在第 " << (it - v.begin()) << " 个" << std::endl;
    }

    auto miss = std::find(v.begin(), v.end(), 100);
    if (miss == v.end()) {
        std::cout << "100 不在里面" << std::endl;
    }

    return 0;
}
