// 练习 4-18 的答案：最高分是谁。
//
// 使用的是「先排序，再取第 0 个元素」这条路径：规矩跟例题一样，两句话——
// 分数高的在前，同分按名字。排完 roster[0] 就是答案：
// 小张 95，小陈也是 95，但「张」的字节比「陈」小，所以小张排在前面。
//
// 另外两条路径同样可行：自己遍历一遍比较（记录位置），
// 或者用 max_element 再自己写同分时的取舍——第一条最直接，
// 第二条要用自定义的比较，都留给题面里说的「写一遍」。
//
// 名单从输入读：第一行 n，后面每行一个「名字 分数」。

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int score;
};

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<Student> roster;
    for (int i = 0; i < n; i++) {
        Student s;
        std::cin >> s.name >> s.score;
        roster.push_back(s);
    }

    std::sort(roster.begin(), roster.end(),
              [](const Student& a, const Student& b) {
                  if (a.score != b.score) {
                      return a.score > b.score;
                  }
                  return a.name < b.name;
              });

    std::cout << "最高分是 " << roster[0].score << std::endl;
    std::cout << "他是 " << roster[0].name << std::endl;

    return 0;
}
