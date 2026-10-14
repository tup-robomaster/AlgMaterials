// 6.5 为什么 sizeof 在函数里就变小了
//
// main 里 a 是「一排三个 int」，占 12 格。
// 进了函数，参数表上写的还是 int a[3]，可它其实只是一个 int*——
// 收的是门牌号，不是那一排本身。所以 sizeof 变成了 8（一个指针的大小）。
//
// 这正解释了 4.x 里那个「数组传进函数就能被改」的现象：
// 函数拿到的就是那一排本身的门牌号，不是抄件。

#include <iostream>

void show(int a[3]) {
    std::cout << "函数里 sizeof(a) = " << sizeof(a) << std::endl;
}

int main() {
    int a[3] = {10, 20, 30};

    std::cout << "main 里 sizeof(a) = " << sizeof(a) << std::endl;

    show(a);

    return 0;
}
