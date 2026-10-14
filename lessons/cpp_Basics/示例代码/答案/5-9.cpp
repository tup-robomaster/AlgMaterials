// 练习 5-9 的答案：给 Point 加构造函数。
//
// 参数名跟成员名一样，所以两句都得写 this->。
// 不写的话就是「参数赋给参数」，跟 5.3 里那份错的一模一样。

#include <iostream>

struct Point {
    int x;
    int y;

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void print() {
        std::cout << x << " " << y << std::endl;
    }
};

int main() {
    Point p(3, 4);

    p.print();

    return 0;
}
