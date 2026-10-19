// 3.4：map 里的东西，不用你排，它自己就是按 key 排好的。
//
// 塞进去的顺序是乱的，打出来的顺序是有序的——这是 map 跟 vector 最大的不同。
//
// 这一份故意把那个类型名写全：下一节讲它为什么长这样，以及怎么省掉它。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.4-map-order.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> counts;

    counts["pear"] = 3;
    counts["apple"] = 4;
    counts["banana"] = 5;

    for (const std::pair<const std::string, int>& kv : counts) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }

    std::cout << "---" << std::endl;

    std::map<int, std::string> names;

    names[102] = "小红";
    names[101] = "小明";
    names[103] = "小刚";

    for (const std::pair<const int, std::string>& kv : names) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }

    return 0;
}
