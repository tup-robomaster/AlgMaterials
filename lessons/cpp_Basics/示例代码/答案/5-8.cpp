// 练习 5-8 的答案：把名单那份改成 class 版，打印折进成员函数里。
//
// 跟 5.1 例题比：struct 换成 class、顶上加了 public:、
// 那个长长的 cout 搬进了 print()。
// main 里最后那个循环，从「每次都写三样」变成了 s[i].print();。

#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    int id;
    int score;

    void print() {
        std::cout << name << " " << id << " " << score << std::endl;
    }
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
        s[i].print();
    }

    return 0;
}
