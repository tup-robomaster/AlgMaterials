// 析构函数：对象死的时候自动跑的那一段
//
// 出生打一行、死亡打一行，就能看着它什么时候跑。
// 局部对象死在它所在的那个函数结束的时候——speak() 里那个死在 speak() 结束，
// main 里那个死在 main 结束。所以「临时同学」比「小明」先走。

#include <iostream>
#include <string>

class Student {
public:
    std::string name;

    Student(std::string n) {
        name = n;
        std::cout << name << " 出生了" << std::endl;
    }

    ~Student() {
        std::cout << name << " 没了" << std::endl;
    }
};

void speak() {
    Student t("临时同学");

    std::cout << t.name << " 说了一句话" << std::endl;
}

int main() {
    Student a("小明");
    Student c("小刚");

    speak();

    std::cout << "main 要结束了" << std::endl;

    return 0;
}
