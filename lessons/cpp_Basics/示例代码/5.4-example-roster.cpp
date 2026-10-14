// 5.4 继承 —— 例题：名单里多了一种人
//
// 5.1 / 5.3 那份名单是三个本科生，现在加一个研究生。
// 本科生那三行一个字都没动，研究生只写自己新加的东西。
//
// 「学生」这件事（名字、学号、成绩、怎么打）从此只有一处定义。

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
    Student s[3] = {
        Student("小明", 101, 90),
        Student("小红", 102, 85),
        Student("小刚", 103, 95)
    };

    for (int i = 0; i < 3; i++) {
        s[i].print();
    }

    GradStudent g("小美", 201, 88, "王老师");
    g.print();

    return 0;
}
