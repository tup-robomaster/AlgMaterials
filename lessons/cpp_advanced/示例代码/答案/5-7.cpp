// 练习 5-7 的答案：同一件事交给 unique_ptr。
//
// 和 5.4 的例题是同一种写法，换来的是整份程序里没有 delete——
// 「谁销毁」这件事写在了类型上：s 一销毁，名单上那个学生随之出名单。
// 析构函数里那行打印就是为了让这件事可见。

#include <iostream>
#include <memory>
#include <string>

class Student {
public:
    Student(std::string name, int score) : name(name), score(score) {
        std::cout << "（" << name << " 进了名单）" << std::endl;
    }

    ~Student() {
        std::cout << "（" << name << " 出了名单）" << std::endl;
    }

    void add(int n) {
        score = score + n;
    }

    void show() const {
        std::cout << name << " " << score << std::endl;
    }

private:
    std::string name;
    int score;
};

int main() {
    std::unique_ptr<Student> s = std::make_unique<Student>("小李", 90);

    s->add(5);
    s->show();

    std::cout << "—— 主函数要结束了 ——" << std::endl;

    return 0;
}
