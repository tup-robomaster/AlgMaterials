// 练习 5-9 的答案：遍历一遍 map，把键和值分别命名。
//
// 两趟遍历的写法只差一个 &：
//   第一遍 const auto& ——只读不改，副本也好原对象也好，取最快的写法；
//   第二遍 auto&       ——要修改的是 map 里的元素，那个 & 不能少。
// 少写了 &，修改的是复制出来的副本，循环一结束就销毁，map 里一个数都不会变。

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> score;
    score["小李"] = 92;
    score["小王"] = 85;
    score["小张"] = 77;

    for (const auto& [name, s] : score) {
        std::cout << name << " = " << s << std::endl;
    }

    std::cout << "—— 每人加 3 分 ——" << std::endl;

    for (auto& [name, s] : score) {
        s = s + 3;
    }

    for (const auto& [name, s] : score) {
        std::cout << name << " = " << s << std::endl;
    }

    return 0;
}
