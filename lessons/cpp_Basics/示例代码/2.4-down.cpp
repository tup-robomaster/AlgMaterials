// 2.4：倒着数。
//
// 起点 5、条件 >= 1、第三格减 1——三件事一起往下走。

#include <iostream>

int main() {
    for (int i = 5; i >= 1; i = i - 1) {
        std::cout << i << std::endl;
    }

    return 0;
}
