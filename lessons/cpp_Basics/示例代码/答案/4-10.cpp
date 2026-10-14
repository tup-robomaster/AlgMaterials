// 练习 4-10 的答案：把 int maxScore = 0; 改成 int maxScore = score[0];
//
// 只动这一行。0 不是这个数组里的元素，它一上台就再也没下来过——
// 五个数全是负的，没有谁比 0 大，那个 if 一次都没进去。
//
// 循环这里保持 i = 0 没动：从 0 起不会错，只是第 0 个会跟自己打一场。

#include <iostream>

int main() {
    int score[5] = {-90, -85, -77, -60, -95};

    int maxScore = score[0];

    for (int i = 0; i < 5; i++) {
        if (score[i] > maxScore) {
            maxScore = score[i];
        }
    }

    std::cout << maxScore << std::endl;

    return 0;
}
