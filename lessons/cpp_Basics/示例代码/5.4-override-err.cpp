// 跟 5.4-override-typo.cpp 是同一个程序，只多了 override 这一个词
//
// 这份编译不过——因为 override 的意思是「我这是在盖父类那个函数，
// 你替我核一下」。编译器一核：父类里没有一个叫 Print 的虚函数，
// 当场把这一处指出来。
//
// 故意写得极简（成员和打印都在，但一个多余的东西都没有），
// 为的是让报错里的行号能和课里贴的源码逐行对上。

#include <iostream>
#include <string>

class Student {
public:
    std::string name;

    Student(std::string n) {
        name = n;
    }

    virtual void print() {
        std::cout << name << std::endl;
    }
};

class GradStudent : public Student {
public:
    GradStudent(std::string n) : Student(n) {}

    void Print() override {
        std::cout << name << "（研究生）" << std::endl;
    }
};

void show(Student& s) {
    s.print();
}

int main() {
    GradStudent b("小红");

    show(b);

    return 0;
}
