// 3.4：函数拿到的是复印件。
//
// 和 3.3-scope.cpp 摆在一起看：那一个讲 x 什么时候消失，
// 这一个讲 x 从哪来——它是 a 的复印件，不是 a 本人。
//
// 这一份就是课上那个动画跑的程序。
//
// 跑一下：g++ -std=c++17 -Wall 示例代码/3.4-copy.cpp -o /tmp/t && /tmp/t

#include <iostream>

void addOne(int x) {
    x = x + 1;
    std::cout << "函数里 x = " << x << std::endl;
}

int main() {
    int a = 3;
    addOne(a);
    std::cout << "回来 a = " << a << std::endl;
    return 0;
}
