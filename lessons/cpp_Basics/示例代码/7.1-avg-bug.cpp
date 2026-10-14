// 7.1 程序不对时，按什么顺序找 —— 正文里那份「总分不对」的程序。
//
// 五个人的分数，本来该打 400 和 80，它却打 335 和 67。
// 病根在循环那一行的上限上，只差一个数字。

#include <iostream>

int main() {
    int scores[5] = {80, 91, 76, 88, 65};
    int sum = 0;

    for (int i = 0; i < 4; i++) {
        sum = sum + scores[i];
    }

    int avg = sum / 5;

    std::cout << "总分：" << sum << std::endl;
    std::cout << "平均分：" << avg << std::endl;

    return 0;
}
