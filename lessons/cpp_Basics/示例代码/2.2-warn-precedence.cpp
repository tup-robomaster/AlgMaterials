// 2.2：&& 比 || 紧，所以不加括号的两句和加了括号的两句不是一回事。
//
// a 是 1，b 是 9。两个 if 长得几乎一样，走的路不一样。

#include <iostream>

int main() {
    int a = 1;
    int b = 9;

    if (a == 1 || a == 2 && b == 3) {
        std::cout << "没括号的：进来了" << std::endl;
    } else {
        std::cout << "没括号的：没进来" << std::endl;
    }

    if ((a == 1 || a == 2) && b == 3) {
        std::cout << "有括号的：进来了" << std::endl;
    } else {
        std::cout << "有括号的：没进来" << std::endl;
    }

    return 0;
}
