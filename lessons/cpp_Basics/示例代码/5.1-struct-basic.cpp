// 5.1 struct —— 一个人就是一个盒子
//
// 把姓名、学号、成绩三样捆成一样东西，起个名字叫 Student。
// 从此「一个学生」就是一个盒子，不用再靠下标去对齐三排数组。

#include <iostream>
#include <string>

struct Student {
    std::string name;
    int id;
    int score;
};

int main() {
    Student a;

    a.name = "小明";
    a.id = 101;
    a.score = 90;

    std::cout << a.name << " " << a.id << " " << a.score << std::endl;

    return 0;
}
