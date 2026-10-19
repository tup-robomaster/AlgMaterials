// 3.4：上一份那两行写得手酸的类型名，换成 auto。
//
// auto 顶掉的是「类型」那一段，const 和 & 照样自己写——
// 所以最后那圈故意不写 const，值就改得动了。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.4-map-auto.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> counts;

    counts["pear"] = 3;
    counts["apple"] = 4;
    counts["banana"] = 5;

    for (const auto& kv : counts) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }

    std::cout << "---" << std::endl;

    std::map<int, std::string> names;

    names[102] = "小红";
    names[101] = "小明";
    names[103] = "小刚";

    for (const auto& kv : names) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }

    std::cout << "---" << std::endl;

    for (auto& kv : counts) {            // 这回没写 const，kv.second 改得动
        kv.second = kv.second + 10;
    }

    for (const auto& kv : counts) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }

    return 0;
}
