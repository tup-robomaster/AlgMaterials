// 练习 3-4 的答案：printStars——打一行 n 个星号。
//
// 和 3.2 的 printTriangle 是一对：那个打很多行，这个打一行。
// 打一整行这件事本身，也是「一件事」，也能起名字。

#include <iostream>

void printStars(int n) {
    for (int i = 1; i <= n; i = i + 1) {
        std::cout << "*";
    }
    std::cout << std::endl;
}

int main() {
    printStars(8);
    printStars(3);

    return 0;
}
