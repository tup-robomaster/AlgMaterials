// 练习 4-8 的答案：找最大的那个。
//
// 台子上第一个人从数组里请——a[0]，不是 0。
// 这一题只要值，所以台上放的是分数本身。

#include <iostream>

int main() {
    int a[5] = {7, 0, 5, 2, 9};

    int maxScore = a[0];

    for (int i = 1; i < 5; i++) {
        if (a[i] > maxScore) {
            maxScore = a[i];
        }
    }

    std::cout << maxScore << std::endl;

    return 0;
}
