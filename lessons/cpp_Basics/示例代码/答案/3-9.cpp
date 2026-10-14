// 练习 3-9 的答案：a 从头到尾都是 6。
//
// doubleIt 收下的是 a 的复印件（装进自己的 x），在里面乘了 2。
// 复印件变成 12，原件 a 一动不动。

#include <iostream>

void doubleIt(int x) {
    x = x * 2;
    std::cout << "函数里 x = " << x << std::endl;
}

int main() {
    int a = 6;
    doubleIt(a);
    std::cout << "回来 a = " << a << std::endl;
    return 0;
}
