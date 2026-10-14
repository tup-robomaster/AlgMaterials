// 派生类对象：出生和死亡的先后
//
// 装的时候从里往外（先父类那部分），拆的时候从外往里（先子类那部分）。
// 这里 Student 有不收参数的构造函数，所以 GradStudent 的构造函数
// 不用写冒号后面那一句——不写也找得到父类的默认构造函数。
//
// 两类各打两行，一共四行，顺序一眼就能看见。

#include <iostream>

class Student {
public:
    Student() {
        std::cout << "先装学生那部分" << std::endl;
    }

    ~Student() {
        std::cout << "最后拆学生那部分" << std::endl;
    }
};

class GradStudent : public Student {
public:
    GradStudent() {
        std::cout << "再装研究生那部分" << std::endl;
    }

    ~GradStudent() {
        std::cout << "先拆研究生那部分" << std::endl;
    }
};

int main() {
    GradStudent b;

    std::cout << "—— 出生结束，下面是死亡 ——" << std::endl;

    return 0;
}
