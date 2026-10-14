// 5.1 struct —— 例题：一张三个人的名单
//
// struct 和数组套在一起用：一排盒子，每个盒子里装着一整份学生信息。

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

    for (int i = 0; i < 3; i++) {
        std::cout << s[i].name << " " << s[i].id << " " << s[i].score << std::endl;
    }

    return 0;
}
