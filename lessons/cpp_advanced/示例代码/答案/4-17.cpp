// 练习 4-17 的答案：按分数从低到高排。
//
// 跟例题只差一个符号：例题那条规矩里写的是 a.score > b.score（高的在前），
// 这儿换成 <（低的在前），整张表就翻过来了。
// 第二句一个字没动——分数一样还是按名字排，所以两对同分的顺序
// 跟例题里完全相同，只是整张表反过来了。
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
                      return a.score < b.score;
                  }
                  return a.name < b.name;
              });

    for (const Student& s : roster) {
        std::cout << s.name << " " << s.score << std::endl;
    }

    return 0;
}
