// 2.4：for 的第三格空着。
//
// i 一直是 1，条件永远成立。跑起来和 2.3 那个 while 版一模一样：
// 一行一行地打 1，停不下来。要停只能按 Ctrl+C 中止。

#include <iostream>

int main() {
    for (int i = 1; i <= 5;) {
        std::cout << i << std::endl;
    }

    return 0;
}
