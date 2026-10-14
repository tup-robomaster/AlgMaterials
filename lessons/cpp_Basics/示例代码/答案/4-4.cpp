// 练习 4-4 的答案：遍历打印。
//
// 五个盒子，合法编号 0 到 4，所以条件是 i < 5。

#include <iostream>

int main() {
    int a[5] = {7, 0, 5, 2, 9};

    for (int i = 0; i < 5; i++) {
        std::cout << a[i] << std::endl;
    }

    return 0;
}
