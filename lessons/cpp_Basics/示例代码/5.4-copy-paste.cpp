// 5.4 继承 —— 起点：两个类，一半是抄的
//
// 研究生也是学生，只是多一个导师。于是 Student 里那三样东西、
// 构造函数、print()，全被原样抄了一遍，再各自加上导师那一行。
//
// 这份能跑，输出也对。问题不在能不能跑，在「以后改哪儿」。

#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    int id;
    int score;

    Student(std::string n, int i, int s) {
        name = n;
        id = i;
        score = s;
    }

    void print() {
        std::cout << name << " " << id << " " << score << std::endl;
    }
};

class GradStudent {
public:
    std::string name;
    int id;
    int score;
    std::string advisor;

    GradStudent(std::string n, int i, int s, std::string adv) {
        name = n;
        id = i;
        score = s;
        advisor = adv;
    }

    void print() {
        std::cout << name << " " << id << " " << score << std::endl;
        std::cout << "导师：" << advisor << std::endl;
    }
};

int main() {
    Student a("小明", 101, 90);
    a.print();

    GradStudent b("小红", 102, 85, "王老师");
    b.print();

    return 0;
}
