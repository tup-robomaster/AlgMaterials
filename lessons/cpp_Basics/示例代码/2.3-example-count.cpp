// 2.3 例题：打 1 到 5，用 while。
//
// 那五行塌成了三行。把 5 改成 100，它就会打 1 到 100。

#include <iostream>

int main() {
    int i = 1;

    while (i <= 5) {
        std::cout << i << std::endl;
        i = i + 1;
    }

    return 0;
}
