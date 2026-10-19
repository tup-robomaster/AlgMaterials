// 3.4：map——用中括号，以键作为下标。
//
// 跟 vector 的中括号比，差在方括号里写什么：vector 写序号，map 写 key。
// 序号得连着、得从 0 起；key 随便，是几就是几。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.4-map-basic.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> counts;

    counts["apple"] = 3;         // 本来没有，新建一个
    counts["banana"] = 5;
    counts["apple"] = 4;         // 本来有，改掉它

    std::cout << counts.size() << std::endl;
    std::cout << counts["apple"] << std::endl;

    std::cout << counts.count("banana") << std::endl;   // 在不在：1
    std::cout << counts.count("cherry") << std::endl;   // 在不在：0

    counts.erase("banana");      // 键和对应的值一起删除

    std::cout << counts.count("banana") << std::endl;
    std::cout << counts.size() << std::endl;

    return 0;
}
