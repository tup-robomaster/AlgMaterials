// 练习 5-2 的答案：申请一块内存、使用一次、释放——封装成函数，调用三次。
//
// 「配对」这两个字是这一课的纪律：new 和 delete 写在同一个函数里，
// 谁看这一段都能一眼数出来两边一样多。申请和释放分散在两个函数里，
// 迟早会有一边遗漏。

#include <iostream>

void show_double(int x) {
    int* p = new int;
    *p = x * 2;

    std::cout << x << " 翻一倍是 " << *p << std::endl;

    delete p;
}

int main() {
    show_double(6);
    show_double(15);
    show_double(100);

    return 0;
}
