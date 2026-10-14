// 5.3 构造函数 —— 起点：对象出生的时候，里面是什么
//
// 开一个 Student，什么都不给。编译干净，一个警告都没有。
// 跑出来 id 和 score 每次都不一样——那两个格子里装着的是
// 栈上留下来的旧东西（4.1 见过这件事）。

#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    int id;
    int score;
};

int main() {
    Student a;

    std::cout << a.id << " " << a.score << std::endl;

    return 0;
}
