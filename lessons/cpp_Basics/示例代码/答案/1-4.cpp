// 练习 1-4 的答案：输出 8。
//
// c 在 b 被改成 100 之前，就已经把 3 + 5 的结果 8 装进去了。
// 往后 b 再变成什么，c 都不管——盒子和盒子之间没有牵连。

#include <iostream>

int main() {
    int a = 3;
    int b = 5;
    int c = a + b;

    b = 100;

    std::cout << c << std::endl;
    return 0;
}
