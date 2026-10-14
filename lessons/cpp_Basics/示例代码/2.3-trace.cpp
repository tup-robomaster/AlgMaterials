// 2.3：把每一步都打出来，看循环是怎么转的。
//
// 这是「用纸笔跟一遍循环」的机器版——你算的和它打的对不对得上，一试就知道。

#include <iostream>

int main() {
    int i = 1;

    while (i <= 3) {
        std::cout << "进循环时 i = " << i << std::endl;
        i = i + 1;
        std::cout << "加完 1 之后 i = " << i << std::endl;
    }

    std::cout << "循环结束后 i = " << i << std::endl;

    return 0;
}
