// 练习 3-10 的答案：参数表上补一个 &。
//
// 函数体一行没动——错的地方从来不在函数里面，在括号里。

#include <iostream>

void doubleIt(int& x) {
    x = x * 2;
}

int main() {
    int a = 6;
    doubleIt(a);
    std::cout << "现在 a = " << a << std::endl;
    return 0;
}
