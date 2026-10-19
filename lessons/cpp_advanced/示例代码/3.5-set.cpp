// 3.5：set——只问「在不在」的容器。重复塞进去的，它不理你。
//
// 它跟 map 是一家人：有序、不重复。差别是 map 每一项有两样（key + 值），
// set 只有一样——那一样自己就是 key。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.5-set.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <set>
#include <string>

int main() {
    std::set<int> s;

    s.insert(5);
    s.insert(3);
    s.insert(5);         // 已经有了
    s.insert(9);
    s.insert(3);         // 也有了

    std::cout << s.size() << std::endl;

    for (int x : s) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::cout << s.count(5) << std::endl;
    std::cout << s.count(4) << std::endl;

    s.erase(5);
    std::cout << s.size() << std::endl;

    std::set<std::string> words;

    words.insert("pear");
    words.insert("apple");
    words.insert("banana");
    words.insert("apple");

    for (const std::string& w : words) {
        std::cout << w << " ";
    }
    std::cout << std::endl;

    return 0;
}
