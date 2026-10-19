// 3.4：中括号是会动手的——问一句「cherry 几次」，它顺便把 cherry 建出来了。
//
// vector 那边越界是读别人的地方，map 这边不越界，它干脆创建一个。
// 想知道「在不在」用 count，别用中括号。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.4-map-trap.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> counts;
    counts["apple"] = 3;

    int x = counts["cherry"];      // 只是想问一下
    std::cout << x << std::endl;

    std::cout << counts.size() << std::endl;   // 多出来一个

    for (const auto& kv : counts) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }

    return 0;
}
