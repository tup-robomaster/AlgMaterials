// 3.2 的例题：把 2.1 那道「及格还是不及格」的判断包成函数。
//
// main 里那两个 if 开头长得一模一样，差的就是括号里那个分数。
// 判断规则只有一份，写在 isPass 里。

#include <iostream>

bool isPass(int score) {
    if (score >= 60) {
        return true;
    }
    return false;
}

int main() {
    int score = 58;
    if (isPass(score)) {
        std::cout << score << " 分：及格" << std::endl;
    } else {
        std::cout << score << " 分：不及格" << std::endl;
    }

    score = 91;
    if (isPass(score)) {
        std::cout << score << " 分：及格" << std::endl;
    } else {
        std::cout << score << " 分：不及格" << std::endl;
    }

    return 0;
}
