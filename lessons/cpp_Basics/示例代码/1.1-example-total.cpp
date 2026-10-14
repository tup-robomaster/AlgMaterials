// 1.1 例题：开三个盒子，算一次加法
//
// 一份外卖 25 元，一份奶茶 18 元，算总价。

#include <iostream>

int main() {
    int meal = 25;              // 开一个盒子叫 meal，放 25
    int tea = 18;               // 开一个盒子叫 tea，放 18
    int total = meal + tea;     // 先算右边（25 + 18 = 43），再把 43 放进 total

    std::cout << total << std::endl;

    return 0;
}
