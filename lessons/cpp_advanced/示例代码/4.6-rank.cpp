// 4.6 例题：排行榜——从高到低，分数一样就按名字排。
//
// 那条 lambda 里可以写不止一句：分数不一样，比分数；分数一样，再比名字。
// 第二句不是可有可无的——没有它，两个同分的人谁在前面就无法确定，
// 同一份输入多运行几次，名单可能不一样。
//
// 第三行的名次就是「它在第几个」：下标从 0 计数，名次从 1 计数，相差一个。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.6-rank.cpp -o /tmp/t && /tmp/t

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int score;
};

int main() {
    std::vector<Student> roster = {{"小李", 90}, {"小张", 95}, {"小王", 90},
                                   {"小赵", 85}, {"小陈", 95}};

    std::sort(roster.begin(), roster.end(),
              [](const Student& a, const Student& b) {
                  if (a.score != b.score) {
                      return a.score > b.score;
                  }
                  return a.name < b.name;
              });

    for (int i = 0; i < int(roster.size()); i++) {
        std::cout << "第 " << i + 1 << " 名  " << roster[i].name << " "
                  << roster[i].score << std::endl;
    }

    return 0;
}
