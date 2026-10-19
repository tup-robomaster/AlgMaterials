// 2.2 用：和 2.2-最小cmake 那份是同一个程序，一个字没错。
//
// 这一份里炸的是旁边 CMakeLists.txt 的第一行——cmake 读到那儿就不往下走了，
// 这个程序根本没轮到编译。所以这份目录故意只有三行命令，没有 make。

#include <iostream>

int main() {
    std::cout << "这份程序是 cmake 编译得到的。" << std::endl;
    std::cout << "四行 CMakeLists.txt，替换了手动输入的那串参数。" << std::endl;
    return 0;
}
