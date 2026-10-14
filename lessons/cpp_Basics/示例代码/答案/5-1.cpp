// 练习 5-1 的答案：定义 struct Point，装一个点。
//
// struct 里那两行是「这个类型里有什么」，末尾的分号别忘——
// 大括号后面那个分号是整个定义的一部分。

#include <iostream>

struct Point {
    int x;
    int y;
};

int main() {
    Point p;

    p.x = 3;
    p.y = 4;

    std::cout << p.x << " " << p.y << std::endl;

    return 0;
}
