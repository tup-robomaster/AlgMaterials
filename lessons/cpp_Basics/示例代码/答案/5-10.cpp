// 练习 5-10 的答案：把坐标藏起来，留两道门。
//
// x 和 y 挪到 private 下面，外面再写 p.x 就是 5.3 里那个
// is private within this context。要读只能走 getX() / getY()。

#include <iostream>

class Point {
private:
    int x;
    int y;

public:
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }
};

int main() {
    Point p(3, 4);

    std::cout << p.getX() << " " << p.getY() << std::endl;

    return 0;
}
