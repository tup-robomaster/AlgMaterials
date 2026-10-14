// 练习 2-15 答案：1 到 100 之间所有偶数的和。
//
// 这一题的重点是第三格：i = i + 2，一步一步跳着走，
// 压根碰不到奇数——不用在循环里写 if 挑一遍。

#include <iostream>

int main() {
    int sum = 0;

    for (int i = 2; i <= 100; i = i + 2) {
        sum = sum + i;
    }

    std::cout << sum << std::endl;

    return 0;
}
