// 7.1 程序不对时，按什么顺序找 —— 上面那份改对的样子。
//
// 只动了一个数字：i < 4 改成 i < 5。

#include <iostream>

int main() {
    int scores[5] = {80, 91, 76, 88, 65};
    int sum = 0;

    for (int i = 0; i < 5; i++) {
        sum = sum + scores[i];
    }

    int avg = sum / 5;

    std::cout << "总分：" << sum << std::endl;
    std::cout << "平均分：" << avg << std::endl;

    return 0;
}
