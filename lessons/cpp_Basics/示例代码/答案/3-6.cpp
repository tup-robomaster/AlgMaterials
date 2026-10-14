// 练习 3-6 的答案：改好的那一份。
//
// 和 3-6-err.cpp 只差一个实参：调用的时候把数字补上。
// 函数本身一个字没动——错的地方从来不在函数里。

#include <iostream>

void printStars(int n) {
    for (int i = 1; i <= n; i = i + 1) {
        std::cout << "*";
    }
    std::cout << std::endl;
}

int main() {
    printStars(5);
    return 0;
}
