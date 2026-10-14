// 5.2 class —— 例题：打印的时候顺便说一句及格没有
//
// 两个成员函数，一个用另一个：print() 里直接写了 isPass()，
// 前面没有 s. —— 它俩住在同一个盒子里，互相说话不用点名。

#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    int id;
    int score;

    bool isPass() {
        return score >= 60;
    }

    void print() {
        std::cout << name << " " << score << " ";
        if (isPass()) {
            std::cout << "及格" << std::endl;
        } else {
            std::cout << "不及格" << std::endl;
        }
    }
};

int main() {
    Student s[3];

    s[0].name = "小明";
    s[0].score = 90;

    s[1].name = "小红";
    s[1].score = 55;

    s[2].name = "小刚";
    s[2].score = 77;

    for (int i = 0; i < 3; i++) {
        s[i].print();
    }

    return 0;
}
