// 4.6：给 sort 一条 lambda，它就知道按什么排了。
//
// 那条 lambda 回答的是同一个问题：「a 该排在 b 前面吗」。
// 按分数从高到低是 a.score > b.score；
// 按名字排是 a.name < b.name（string 自己就有 <，比的是字节，4.4 学的）。
//
// 输出三遍名单，抽取成一个函数——同一段操作写第二遍就应该定义名称了。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.6-sort-by.cpp -o /tmp/t && /tmp/t

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int score;
};

void print(const std::vector<Student>& roster) {
    for (const Student& s : roster) {
        std::cout << s.name << " " << s.score << std::endl;
    }
}

int main() {
    std::vector<Student> roster = {{"小李", 90}, {"小王", 77},
                                   {"小张", 95}, {"小赵", 85}};

    std::cout << "原来的顺序：" << std::endl;
    print(roster);

    std::sort(roster.begin(), roster.end(),
              [](const Student& a, const Student& b) { return a.score > b.score; });

    std::cout << "按分数从高到低：" << std::endl;
    print(roster);

    std::sort(roster.begin(), roster.end(),
              [](const Student& a, const Student& b) { return a.name < b.name; });

    std::cout << "按名字排：" << std::endl;
    print(roster);

    return 0;
}
