// 练习 5-15 的答案：平面点先打，再打 z。
//
// Point3D 自己写了一个 print()，把父类那个盖住了。
// 想叫父类那个，就得点名叫 Point::print()——直接写 print()
// 会自己叫自己，转不出来。

#include <iostream>

class Point {
public:
    int x;
    int y;

    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void print() {
        std::cout << x << " " << y;
    }
};

class Point3D : public Point {
public:
    int z;

    Point3D(int x, int y, int z)
        : Point(x, y) {
        this->z = z;
    }

    void print() {
        Point::print();
        std::cout << " " << z << std::endl;
    }
};

int main() {
    Point3D p(1, 2, 3);

    p.print();

    return 0;
}
