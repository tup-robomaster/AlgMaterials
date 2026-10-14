// 父类引用指着子类对象，而 print 没写 virtual
//
// show() 的参数是「学生的引用」。递进去一个研究生，它也是学生，收得下。
// 可 show 里那句 s.print() 叫的是 Student 那个版本——
// 研究生那行「导师：」不见了。
//
// 跟 5.4-virtual.cpp 是同一个程序，只差 Student::print 前面那个 virtual。

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

void show(Student& s) {
    s.print();
}

int main() {
    Student a("小明", 101, 90);
    GradStudent b("小红", 102, 85, "王老师");

    show(a);
    show(b);

    return 0;
}
