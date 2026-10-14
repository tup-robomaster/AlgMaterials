// 练习 3-8 的答案：两块地盘，两个 step。
//
// 两对大括号各声明了一个 step，一个一个都是新的盒子：
// 第一个装 5，第二个装 7，谁也不认识谁。
// total 住在外面的 main 里，从头活到尾，所以它能一路加下去。

#include <iostream>

int main() {
    int total = 0;

    {
        int step = 5;
        total = total + step;
        std::cout << "第一次之后 total = " << total << std::endl;
    }

    {
        int step = 7;
        total = total + step;
        std::cout << "第二次之后 total = " << total << std::endl;
    }

    return 0;
}
