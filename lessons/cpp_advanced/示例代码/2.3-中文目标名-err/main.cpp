// 2.3 用：让 cmake 报出「target 名字不能用中文」的那份程序。
//
// 这份 main.cpp 从头到尾没被编过——cmake 读 CMakeLists.txt 的时候
// 就停在第 4 行了，根本没走到「叫编译器来」这一步。

#include <iostream>

int main() {
    std::cout << "报表" << std::endl;
    return 0;
}
