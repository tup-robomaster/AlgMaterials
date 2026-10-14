// 7.1 程序不对时，按什么顺序找 —— 砍半砍到底之后剩下的那一份。
//
// 打印、平均分、那两句中文全删了，错照样在：还是 335。
// 九行，毛病就摆在明面上。砍半砍到删不动为止，剩下的就是这个。

#include <iostream>

int main() {
    int scores[5] = {80, 91, 76, 88, 65};
    int sum = 0;

    for (int i = 0; i < 4; i++) {
        sum = sum + scores[i];
    }

    std::cout << sum << std::endl;

    return 0;
}
