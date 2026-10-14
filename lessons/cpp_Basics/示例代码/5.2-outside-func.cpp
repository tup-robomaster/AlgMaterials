// 5.2 class —— 起点：打印这件事写在外面
//
// 3.2 教的写法：把「打印一个学生」包成一个函数，参数收一个 Student。
// 能用，但每次调用都得把整个学生递进去。

#include <iostream>
#include <string>

struct Student {
    std::string name;
    int id;
    int score;
};

void printStudent(Student s) {
    std::cout << s.name << " " << s.id << " " << s.score << std::endl;
}

int main() {
    Student a;
    a.name = "小明";
    a.id = 101;
    a.score = 90;

    printStudent(a);

    return 0;
}
