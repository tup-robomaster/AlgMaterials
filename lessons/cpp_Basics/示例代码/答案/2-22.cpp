// 练习 2-22（找错）：想打一个三角形，打出来是个方块。
//
// 这文件里是**错的**那一版。外圈没错，内圈的上限写错了：
// 写成了 n（固定不变），而不是 i（跟着外圈长）。

#include <iostream>

int main() {
    int n = 3;

    for (int i = 1; i <= n; i = i + 1) {
        for (int j = 1; j <= n; j = j + 1) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    return 0;
}
