// 3.4：例题——一张成绩表，按名字查分。
//
// map 真正好用的地方在这儿：不用一趟一趟找，报个名字就能拿到分。
// 问「有没有这个人」用 count，查到之后才用中括号取。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.4-lookup.cpp -o /tmp/t &&
//         /tmp/t < 示例代码/3.4-lookup.in

#include <iostream>
#include <map>
#include <string>

int main() {
    int n;
    std::cin >> n;

    std::map<std::string, int> scores;

    for (int i = 0; i < n; i++) {
        std::string name;
        int score;
        std::cin >> name >> score;

        scores[name] = score;
    }

    int q;
    std::cin >> q;

    for (int i = 0; i < q; i++) {
        std::string name;
        std::cin >> name;

        if (scores.count(name) > 0) {
            std::cout << name << " " << scores[name] << std::endl;
        } else {
            std::cout << name << " 没这个人" << std::endl;
        }
    }

    return 0;
}
