// 4.3：在数组里找最大的那个。擂台摆起来。
//
// maxScore 先请**第一个**上台——不是 0，理由下面那份反例里讲。
// 然后剩下的挨个上来打：赢了就把它留在台上，输了就下去。
// 全部走完，台上站着的就是最大的。
//
// 循环从 i = 1 起，不是从 0：第一个已经在台上了，不用跟自己打。

#include <iostream>

int main() {
    int score[5] = {90, 85, 77, 60, 95};

    int maxScore = score[0];

    for (int i = 1; i < 5; i++) {
        if (score[i] > maxScore) {
            maxScore = score[i];
        }
    }

    std::cout << maxScore << std::endl;

    return 0;
}
