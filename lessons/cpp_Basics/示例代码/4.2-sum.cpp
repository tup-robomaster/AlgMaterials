// 4.2：遍历加上 2.3 那个攒总和的盒子。
//
// sum 开在循环**外面**——这跟 2.5 练习里那个 x 正好相反。
// x 是「这一圈用完就扔」，sum 是「每一圈都得留着」。
// 开在里面的话，每转一圈它就被清空一次，最后只剩最后一个数。

#include <iostream>

int main() {
    int score[5] = {90, 85, 77, 60, 95};

    int sum = 0;

    for (int i = 0; i < 5; i++) {
        sum = sum + score[i];
    }

    std::cout << sum << std::endl;

    return 0;
}
