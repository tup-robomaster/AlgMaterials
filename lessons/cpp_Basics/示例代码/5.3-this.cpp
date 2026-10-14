// 5.3 构造函数 —— 加上 this-> 就对了
//
// this->name 说的是「我这个盒子的 name」，右边那个 name 是参数。
// 加了箭头，两样东西就分得开了。

#include <iostream>
#include <string>

class Student {
private:
    std::string name;
    int score;

public:
    Student(std::string name, int score) {
        this->name = name;
        this->score = score;
    }

    void print() {
        std::cout << name << " " << score << std::endl;
    }
};

int main() {
    Student a("小明", 90);

    a.print();

    return 0;
}
