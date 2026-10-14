// 练习 5-5 的答案：给 Point 加一个 print()。
//
// 函数搬进去以后，函数体里写的是 x 和 y，前面没有 p. ——
// 它自己就是那个点。

#include <iostream>

struct Point {
    int x;
    int y;

    void print() {
        std::cout << x << " " << y << std::endl;
    }
};

int main() {
    Point p;

    p.x = 3;
    p.y = 4;

    p.print();

    return 0;
}
