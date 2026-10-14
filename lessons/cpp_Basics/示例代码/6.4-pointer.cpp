// 6.4 地址传递：函数拿到的是门牌号
//
// 参数变成 int* p，收的是地址。函数里写 *p 就是写外面那个 a。
// 调用那行要写成 addOne(&a)——**得自己把地址递过去**。

#include <iostream>

void addOne(int* p) {
    *p = *p + 1;
}

int main() {
    int a = 3;

    addOne(&a);

    std::cout << a << std::endl;

    return 0;
}
