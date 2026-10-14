// 练习 1-6 的答案：先打字母，再打 96。
//
// char 盒子里装的其实是一个编号。'a' 的编号是 97。
// c - 1 是拿编号去减，减完是个整数 96，所以打出来是数字。

#include <iostream>

int main() {
    char c = 'a';
    std::cout << c << std::endl;
    std::cout << c - 1 << std::endl;
    return 0;
}
