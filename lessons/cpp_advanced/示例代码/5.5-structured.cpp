// 5.5 例题：结构化绑定——遍历 map 不用再写 .first / .second。
//
// 3.4 里遍历 map 写的是 kv.first、kv.second；这一课起可以直接
// 把两个名字分别命名。加不加 & 是原有规则（3.1 的副本与原对象）：
// 写成 auto&，修改的就是 map 里的元素；不加，修改的是复制出来的副本，循环结束后就销毁。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.5-structured.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> score;
    score["小李"] = 92;
    score["小王"] = 85;
    score["小张"] = 77;

    std::cout << "—— 原名 ——" << std::endl;
    for (const auto& [name, s] : score) {
        std::cout << name << "：" << s << std::endl;
    }

    // 加了 &，这里是 map 里那一份的另一个名字，改的就是它
    for (auto& [name, s] : score) {
        s = s + 5;
    }

    std::cout << "—— 各加 5 分之后 ——" << std::endl;
    for (const auto& [name, s] : score) {
        std::cout << name << "：" << s << std::endl;
    }

    return 0;
}
