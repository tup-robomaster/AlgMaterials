// 4.3：要的是「第几个」，不是「几」。
//
// 4.5 那道综合练习要跟最后一个元素交换，交换得知道**位置**。
// 所以这道题记的不是分数，是编号。
//
// 台子上的比较改成了 score[i] > score[maxIdx]：
// 拿分数比，记住的却是编号。

#include <iostream>

int main() {
    int score[5] = {90, 85, 77, 60, 95};

    int maxIdx = 0;

    for (int i = 1; i < 5; i++) {
        if (score[i] > score[maxIdx]) {
            maxIdx = i;
        }
    }

    std::cout << "最大的在第 " << maxIdx << " 个：" << score[maxIdx] << std::endl;

    return 0;
}
