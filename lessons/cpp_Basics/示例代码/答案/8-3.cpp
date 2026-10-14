// 练习 8-3 答案：矩形和正方形，各说各的名字。
//
// Square 那行冒号后面的 Shape(side, side) 是「先让父类出生」——
// 边长一份当两份用，正方形就这么来的（5.4 学的）。
//
// name() 父类那个带 virtual，子类这份带 override。少了这两个词，
// 下面 printShape 收的是 Shape&，就会叫到父类那份，两行都打「矩形」。
// 5.4 拿 no-virtual 和 virtual 两份摆在一起讲的就是这一件事。

#include <iostream>
#include <string>

class Shape {
private:
    int w;
    int h;

public:
    Shape(int a, int b) {
        w = a;
        h = b;
    }

    virtual std::string name() {
        return "矩形";
    }

    int area() {
        return w * h;
    }
};

class Square : public Shape {
public:
    Square(int side) : Shape(side, side) {}

    std::string name() override {
        return "正方形";
    }
};

void printShape(Shape& s) {
    std::cout << s.name() << " " << s.area() << std::endl;
}

int main() {
    Shape rect(3, 4);
    Square sq(5);

    printShape(rect);
    printShape(sq);

    return 0;
}
