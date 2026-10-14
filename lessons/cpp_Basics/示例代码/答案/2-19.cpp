// 练习 2-19 答案：读 n 和 m，打一个 n 行 m 列的星号矩形。
//
// 两个数分工很清楚：n 是「几行」——外圈管；m 是「一行几个」——内圈管。
// 内圈的上限是两个数里跟外圈无关的那个，所以内圈写 m，不写 i。

#include <iostream>

int main() {
    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    for (int i = 1; i <= n; i = i + 1) {
        for (int j = 1; j <= m; j = j + 1) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    return 0;
}
