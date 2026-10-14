// 名字打错了，又没写 override——编译器一声不吭，程序悄悄跑错
//
// GradStudent 里写的那个函数叫 Print（大写 P），跟父类的 print 不是同一个名字。
// 所以它根本没有盖住父类那个，只是自己新加了一个。
//
// show(b) 想让它打两行，结果只打了一行「小红」。
// 这份编译干净、跑得也挺好——**这才是 override 要救的那种错**。
// 跟 5.4-override-err.cpp 摆在一起看：那份只多写了一个 override，
// 编译器当场就把这一处指出来了。

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

    void Print() {
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
