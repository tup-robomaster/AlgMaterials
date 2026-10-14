// 父类引用指着子类对象，print 前面加了 virtual
//
// 只多了一个词，show(b) 就叫到了研究生自己那份，两行都在。
// 跟 5.4-no-virtual.cpp 摆在一起看，差别的全部就是这个 virtual。
// 这一份里还没有 override——那是下一节的事。

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

    virtual void print() {
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
