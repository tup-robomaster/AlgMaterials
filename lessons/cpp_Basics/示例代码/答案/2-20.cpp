// 练习 2-20 答案：读 n，打一个 n 行的倒直角三角形。
//
// 和例题只差一件事：内圈的上限。例题是「越往下越长」，所以上限是 i；
// 这题是「越往下越短」，上限就该反过来——从 n 往下数。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = n; i >= 1; i = i - 1) {
        for (int j = 1; j <= i; j = j + 1) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }

    return 0;
}
