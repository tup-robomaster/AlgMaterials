// 5.2 例题：箭头 p->name 就是 (*p).name。
//
// 指针指向一个自己创建的对象时，取它内部的成员要分两步：
// 先通过指针找到对象，再取字段——写全了是 (*p).name。
// 那对括号不能省，省了 p.name 就成了「p 自己的字段」，
// 而 p 是个指针，没这个字段。箭头是给这一对括号起的外号。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.2-arrow.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    int score;

    Student(std::string n, int s) {
        name = n;
        score = s;
    }
};

int main() {
    Student* p = new Student("小李", 92);

    std::cout << "写全了：" << (*p).name << " " << (*p).score << std::endl;
    std::cout << "用箭头：" << p->name << " " << p->score << std::endl;

    p->score = 95;      // 改的是堆上那个对象
    std::cout << "改过之后：" << (*p).score << std::endl;

    delete p;
    p = nullptr;

    return 0;
}
