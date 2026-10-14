// 4.2 遍历数组 —— 补充：循环再多转一圈，程序就死了
//
// 和 4.2-write-oob.cpp 是同一件事，只把 <= 5 改成了 <= 6。
// 也就是往数组外面多写一格，压到了编译器放在栈上的哨兵。
// 注意屏幕上的五个数照样打得整整齐齐——崩在收尾的时候。

#include <iostream>

int main() {
    int score[5] = {0, 0, 0, 0, 0};

    for (int i = 0; i <= 6; i++) {      // 只改了这一个数字
        score[i] = 60 + i;
    }

    for (int i = 0; i < 5; i++) {
        std::cout << score[i] << std::endl;
    }

    return 0;
}
