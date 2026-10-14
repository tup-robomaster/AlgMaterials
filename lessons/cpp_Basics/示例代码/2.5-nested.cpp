// 2.5：一个循环套一个循环。
//
// 外圈走 2 次，内圈走 3 次。
// 看清楚：内圈不是「和」外圈轮流走，是外圈每走一步，内圈就整轮走完。

#include <iostream>

int main() {
    for (int i = 1; i <= 2; i = i + 1) {
        for (int j = 1; j <= 3; j = j + 1) {
            std::cout << "外 " << i << " 内 " << j << std::endl;
        }
    }

    return 0;
}
