// 4.1 例题：一组成绩。
//
// 五个盒子挨着放，改其中一个、打其中一个，都靠编号。
// 注意最后两行打的是同一个编号——因为中间那一行把它改过了。

#include <iostream>

int main() {
    int score[5] = {90, 85, 77, 60, 95};

    std::cout << "第一个：" << score[0] << std::endl;
    std::cout << "第四个：" << score[3] << std::endl;

    score[3] = 66;

    std::cout << "改过之后：" << score[3] << std::endl;

    return 0;
}
