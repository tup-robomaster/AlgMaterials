// 练习 2-4（找错）：想按分数打「及格」或「不及格」，结果两条都打了。
//
// 75 分，两句都出来了。编译器一声没吭。

#include <iostream>

int main() {
    int score = 75;

    if (score >= 60)
        std::cout << "及格" << std::endl;
    std::cout << "不及格" << std::endl;

    return 0;
}
