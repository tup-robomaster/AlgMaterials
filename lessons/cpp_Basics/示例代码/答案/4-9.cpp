// 练习 4-9 的答案：找最小的，并说出它在第几个。
//
// 跟 4.3 例题一个写法，只是数据换了。
// 台上记的是编号（minIdx），比的是分数（a[minIdx]）。

#include <iostream>

int main() {
    int a[5] = {7, 0, 5, 2, 9};

    int minIdx = 0;

    for (int i = 1; i < 5; i++) {
        if (a[i] < a[minIdx]) {
            minIdx = i;
        }
    }

    std::cout << "最小的是 " << a[minIdx] << "，在第 " << minIdx << " 个" << std::endl;

    return 0;
}
