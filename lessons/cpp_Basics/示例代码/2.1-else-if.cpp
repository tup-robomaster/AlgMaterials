// 2.1：三个档。else if 是按顺序一个一个问下去的。
//
// 把 score 依次改成 58、75、95 各跑一遍，看哪一行变、哪一行不变。

#include <iostream>

int main() {
    int score = 58;

    if (score >= 90) {
        std::cout << "优秀" << std::endl;
    } else if (score >= 60) {
        std::cout << "及格" << std::endl;
    } else {
        std::cout << "不及格" << std::endl;
    }

    return 0;
}
