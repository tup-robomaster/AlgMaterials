// 2.5：例题。读一个 n，打一个 n 行的直角三角形。
//
// 这是嵌套循环最经典的样子，也是 2.6 那道综合练习的半个身子。
// 关键是内圈的上限：不是写死的数，是外圈那个 i。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 1; i <= n; i = i + 1) {
        for (int j = 1; j <= i; j = j + 1) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    return 0;
}
