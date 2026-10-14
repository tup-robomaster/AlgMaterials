// 5.4 继承 —— 子类改掉父类的做法
//
// 子类里再写一个同名的 print()，就把父类那个盖住了。
// 想叫父类那个，得点名叫 Student::print()。
//
// main 里 a.print() 和 b.print() 名字一模一样，各打各的。

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

class GradStudent : public Student {
public:
    std::string advisor;

    GradStudent(std::string n, int i, int s, std::string adv)
        : Student(n, i, s) {
        advisor = adv;
    }

    void print() {
        Student::print();
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
