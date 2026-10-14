// 练习 5-14 的答案：体积直接拿父类的面积去乘。
//
// volume() 里那句 area() 用的是 Shape 传下来的那个，
// 子类不用重写，也不用写 Shape::area()——没有重名，直接叫就行。

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

    int volume() {
        return area() * depth;
    }
};

int main() {
    Box b(3, 4, 5);

    std::cout << b.volume() << std::endl;

    return 0;
}
