// 练习 5-12 的答案：一个矩形。
//
// 这一课学的东西基本都用上了：private 把宽高藏起来、
// 构造函数负责出生时装好、area() 交回一个数、print() 负责打。
// 外面拿不到宽高本身，只能问它面积是多少。

#include <iostream>

class Rect {
private:
    int w;
    int h;

public:
    Rect(int w, int h) {
        this->w = w;
        this->h = h;
    }

    int area() {
        return w * h;
    }

    void print() {
        std::cout << w << " " << h << " " << area() << std::endl;
    }
};

int main() {
    Rect r(3, 4);

    r.print();

    return 0;
}
