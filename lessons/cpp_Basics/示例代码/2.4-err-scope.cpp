// 2.4：循环结束以后还想用 i。
//
// i 是 for 头上那一格建出来的，它只活在循环里。
// 循环一结束，那个盒子就没了——外面的人看不见它。

#include <iostream>

int main() {
    for (int i = 1; i <= 5; i = i + 1) {
        std::cout << i << std::endl;
    }

    std::cout << "循环完了，i 是 " << i << std::endl;

    return 0;
}
