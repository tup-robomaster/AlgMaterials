// 练习 5-7 的答案：成员函数能改自己的数据。
//
// addScore 里那句 score = score + d; —— 左边那个 score 是盒子里那一个，
// 不是参数。d 才是从外面递进来的。

#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    int score;

    void addScore(int d) {
        score = score + d;
    }

    void print() {
        std::cout << name << " " << score << std::endl;
    }
};

int main() {
    Student s;
    s.name = "小红";
    s.score = 55;

    s.addScore(10);
    s.print();

    return 0;
}
