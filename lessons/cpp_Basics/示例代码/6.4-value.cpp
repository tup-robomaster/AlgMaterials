// 6.4 值传递：函数拿到的是抄件
//
// 函数里 x 确实变成了 4，可那是抄件的 4。a 本人一动没动。
// （3.4 讲过这一份，这里把它摆回来，为了让三种能并排看。）

#include <iostream>

void addOne(int x) {
    x = x + 1;
}

int main() {
    int a = 3;

    addOne(a);

    std::cout << a << std::endl;

    return 0;
}
