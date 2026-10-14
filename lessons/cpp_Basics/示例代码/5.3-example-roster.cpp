// 5.3 构造函数 —— 例题：名单短下来了
//
// 跟 5.1 例题那份比：数据一样是三个人。
// 那一份每个学生要写三行赋值，这一份一行一个。
//
// Student s[3] = { Student("小明", 101, 90), ... }; 这种写法读作
// 「一排三个 Student，每个用后面括号里的东西造出来」。

#include <iostream>
#include <string>

class Student {
private:
    std::string name;
    int id;
    int score;

public:
    Student(std::string n, int i, int s) {
        name = n;
        id = i;
        score = s;
    }

    void print() {
        std::cout << name << " " << id << " " << score << std::endl;
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

    return 0;
}
