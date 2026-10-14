// 6.5 一个经典错法：拿 sizeof 数元素个数（练习 6-13 里的那一份）
//
// 想打出 10 20 30，实际只打了 10 20。原因就是 6.5 那条：
// 参数表上的 int a[] 其实是 int*，sizeof 量到的是一个指针（8 格），
// 不是那一排本身（12 格）。8 / 4 = 2，所以只走了两圈。
//
// 编译器其实看出来了，警告里写得很清楚——注意它把函数签名打成
// 『void printAll(int*)』，等于亲口告诉你那个参数是指针。

#include <iostream>

void printAll(int a[]) {
    int n = sizeof(a) / sizeof(int);

    for (int i = 0; i < n; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int a[3] = {10, 20, 30};

    printAll(a);

    return 0;
}
