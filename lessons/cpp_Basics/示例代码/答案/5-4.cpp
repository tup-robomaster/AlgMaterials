// 练习 5-4 的答案：把第 0 个和第 2 个学生整个换过来。
//
// 跟 4.4 换两个整数是同一件事，三个盒子照样用得上。
// 差别只有一个：盒子里装的东西大了，可写法一个字都没变——
// 这正是 struct 值钱的地方。

#include <iostream>
#include <string>

struct Student {
    std::string name;
    int id;
    int score;
};

int main() {
    Student s[3];

    s[0].name = "小明";
    s[0].id = 101;
    s[0].score = 90;

    s[1].name = "小红";
    s[1].id = 102;
    s[1].score = 85;

    s[2].name = "小刚";
    s[2].id = 103;
    s[2].score = 95;

    Student t = s[0];
    s[0] = s[2];
    s[2] = t;

    for (int i = 0; i < 3; i++) {
        std::cout << s[i].name << " " << s[i].id << " " << s[i].score << std::endl;
    }

    return 0;
}
