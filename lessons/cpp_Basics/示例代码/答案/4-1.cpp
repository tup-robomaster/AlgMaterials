// 练习 4-1 的答案：声明一整排盒子，打头尾两个。
//
// 五个盒子，编号 0 到 4。头是 a[0]，尾是 a[4]——不是 a[5]。

#include <iostream>

int main() {
    int a[5] = {3, 1, 4, 1, 5};

    std::cout << a[0] << std::endl;
    std::cout << a[4] << std::endl;

    return 0;
}
