// 2.4：2.3 那个数到 5 的 while，换 for 写。
//
// 三件事——起点、条件、第三步——全挤在一行里了。

#include <iostream>

int main() {
    for (int i = 1; i <= 5; i = i + 1) {
        std::cout << i << std::endl;
    }

    return 0;
}
