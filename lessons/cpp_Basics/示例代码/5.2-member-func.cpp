// 5.2 class —— 把函数搬进 struct 里
//
// 同一个函数，这次写在 struct 的大括号里面。
// 注意函数体里写的是 name、id、score，前面没有 s. —— 它不用接收那个学生，
// 它自己就是那个学生的一部分。

#include <iostream>
#include <string>

struct Student {
    std::string name;
    int id;
    int score;

    void print() {
        std::cout << name << " " << id << " " << score << std::endl;
    }
};

int main() {
    Student a;
    a.name = "小明";
    a.id = 101;
    a.score = 90;

    a.print();

    return 0;
}
