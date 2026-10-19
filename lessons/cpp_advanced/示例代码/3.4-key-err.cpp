// 3.4：键改不了——就算循环里没写 const，kv.first 也是只读的。
//
// map 每一项的真身是 std::pair<const 键, 值>：那个 const 是 map 自己戴上的，
// 不是你写的，所以你摘不掉它。报错跟 3.3 那句是同一句。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.4-key-err.cpp -o /tmp/t

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> counts;
    counts["apple"] = 3;

    for (auto& kv : counts) {
        kv.first.clear();      // 想把这个键的名字抹掉
    }

    return 0;
}
