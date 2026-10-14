// 5.4 继承 —— 重复的那一份交给父类
//
// GradStudent 后面那个冒号读作「是一种」：研究生是一种学生。
// 写完之后，name / id / score 和 print() 都自动有了，一个字都不用抄。
//
// 自己新加的那个 advisor 才是真正要写的东西。

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

    void printAll() {
        print();
        std::cout << "导师：" << advisor << std::endl;
    }
};

int main() {
    GradStudent b("小红", 102, 85, "王老师");

    b.printAll();

    return 0;
}
