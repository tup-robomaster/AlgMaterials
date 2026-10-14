// 练习 4-2 的答案：改第 3 个，再一个一个打出来。
//
// 「第 3 个」的编号是 2。五行 std::cout 里只有方括号那个数字不一样——
// 这正是下一课那个循环要消掉的东西。

#include <iostream>

int main() {
    int a[5] = {3, 1, 4, 1, 5};

    a[2] = 9;

    std::cout << a[0] << std::endl;
    std::cout << a[1] << std::endl;
    std::cout << a[2] << std::endl;
    std::cout << a[3] << std::endl;
    std::cout << a[4] << std::endl;

    return 0;
}
