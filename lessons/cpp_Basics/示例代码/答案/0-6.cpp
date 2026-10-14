// 练习 0-6 的答案：补上第二行末尾那个分号。
//
// 出错的是第 5 行（int b = 5），但编译器报的是第 6 行。
// 为什么，看 0.3「报错的格式」那一节。

#include <iostream>

int main() {
    int a = 3;
    int b = 5;
    std::cout << a + b << std::endl;
    return 0;
}
