// 2.3：死循环。i 一直是 1，条件永远成立。
//
// 别在你的终端里直接跑这一个——它会一直打 1，停不下来。
// 课里说的停下办法：按住 Ctrl 再按 C。

#include <iostream>

int main() {
    int i = 1;

    while (i <= 5) {
        std::cout << i << std::endl;
    }

    return 0;
}
