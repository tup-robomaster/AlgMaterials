// 练习 2-14 答案：for 版，倒着打 n 到 1。
//
// 2.3 例题「倒着数」用的 while。改成 for，倒着数往下走这件事
// 一眼就能看出来——三件事都写在头上了。

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    for (int i = n; i >= 1; i = i - 1) {
        std::cout << i << std::endl;
    }

    return 0;
}
