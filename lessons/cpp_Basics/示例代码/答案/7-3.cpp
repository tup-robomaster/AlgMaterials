// 练习 7-3（找错）：函数里改了，外面的 a 没变。
//
// 第一处不一样的那一行（打 a 的那句）本身没错——它老老实实把 a 打了出来。
// 病根在参数表上：int x 收的是一份抄件，函数里那句改的是抄件。
// 加上 &，x 就成了 a 的另一个名字，改它等于改 a。

#include <iostream>

void addOne(int& x) {
    x = x + 1;
}

int main() {
    int a = 5;

    addOne(a);

    std::cout << "a = " << a << std::endl;

    return 0;
}
