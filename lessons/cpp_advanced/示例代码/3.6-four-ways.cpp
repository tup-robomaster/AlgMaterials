// 3.6：同一串名字，四种问法，四种容器。
//
// 数据只有一份，读进来的动作也只有一次。四个容器各接一份，
// 因为往后要问的问题不一样——问什么，决定用什么装。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.6-four-ways.cpp -o /tmp/t &&
//         /tmp/t < 示例代码/3.6-four-ways.in

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> all;       // 问「一共几个人次」——全留着，一个不落
    std::set<std::string> who;          // 问「有哪几个人」——去重，还有序
    std::map<std::string, int> times;   // 问「各来了几次」——一个键对应一个值
    std::queue<std::string> order;      // 问「按来的先后处理」——先进先出

    for (int i = 0; i < 6; i++) {
        std::string name;
        std::cin >> name;

        all.push_back(name);
        who.insert(name);
        times[name]++;
        order.push(name);
    }

    std::cout << "一共 " << all.size() << " 人次" << std::endl;

    std::cout << "有这几个人：";
    for (const std::string& s : who) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    std::cout << "各来了几次：" << std::endl;
    for (const auto& kv : times) {
        std::cout << "  " << kv.first << " " << kv.second << std::endl;
    }

    std::cout << "按来的先后处理：" << std::endl;
    while (!order.empty()) {
        std::cout << "  " << order.front() << std::endl;
        order.pop();
    }

    return 0;
}
