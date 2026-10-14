// 1.2 类型 —— 补充：char 其实是一个很小的整数
//
// 每个字符在计算机里都有一个编号。'A' 是 65，'B' 是 66，'a' 是 97。
// 所以同一个 char 盒子，你既可以把它当字母看，也可以把它当数字看。

#include <iostream>

int main() {
    char c = 'A';

    std::cout << c << std::endl;          // 直接打，是个字母
    std::cout << c + 1 << std::endl;      // 加 1 再打 —— 加号把它当成了数字
    char next = c + 1;          // 结果放回 char 盒子
    std::cout << next << std::endl;       // 又变回字母了

    int code = c;               // 把 c 的值放进一个 int 盒子
    std::cout << code << std::endl;

    return 0;
}
