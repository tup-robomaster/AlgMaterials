// 练习 5-13 的答案：Box 是一种 Shape。
//
// 构造函数后面那个冒号，是把 w 和 h 直接递给父类的那个构造函数，
// 让父类先把自己那部分装好；depth 才是子类自己装。
// area() 一个字都没写，它是从 Shape 那儿继承来的。

#include <iostream>

class Shape {
public:
    int w;
    int h;

    Shape(int w, int h) {
        this->w = w;
        this->h = h;
    }

    int area() {
        return w * h;
    }
};

class Box : public Shape {
public:
    int depth;

    Box(int w, int h, int depth)
        : Shape(w, h) {
        this->depth = depth;
    }
};

int main() {
    Box b(3, 4, 5);

    std::cout << b.w << " " << b.h << " " << b.depth << " " << b.area() << std::endl;

    return 0;
}
