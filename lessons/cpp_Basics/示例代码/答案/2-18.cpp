// 练习 2-18（找错）：想打 1 到 5，结果打出来是 0 到 4。
//
// 这文件里是**错的**那一版。圈数没错，是 5 圈；
// 错的是「数出来的值」——从 0 起，就得到 0 到 4。

#include <iostream>

int main() {
    for (int i = 0; i < 5; i = i + 1) {
        std::cout << i << std::endl;
    }

    return 0;
}
