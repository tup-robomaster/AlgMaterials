// 练习 2-21 答案：读 n，打一个数字三角形。
//
// 外形和例题一模一样，只是把内圈打的星号换成了 j 本身。
// 所以第 3 行打出来就是 1 2 3。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 1; i <= n; i = i + 1) {
        for (int j = 1; j <= i; j = j + 1) {
            std::cout << j;
        }
        std::cout << std::endl;
    }

    return 0;
}
