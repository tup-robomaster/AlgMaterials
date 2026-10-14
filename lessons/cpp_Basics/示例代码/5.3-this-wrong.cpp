// 5.3 构造函数 —— 忘了写 this-> 会怎样
//
// 参数名和成员名起成了一样的。函数体里那句 name = name; 是
// 「参数赋给参数」，成员那个 name 一个字都没被碰过。
//
// 编译干净，一个警告都没有。跑出来名字是空的，
// 分数是个每次都不一样的垃圾数。

#include <iostream>
#include <string>

class Student {
private:
    std::string name;
    int score;

public:
    Student(std::string name, int score) {
        name = name;
        score = score;
    }

    void print() {
        std::cout << "[" << name << "] " << score << std::endl;
    }
};

int main() {
    Student a("小明", 90);

    a.print();

    return 0;
}
