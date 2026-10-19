// 2.2 用：第一份 CMakeLists.txt 要编的那个程序。
//
// 程序本身没有新东西——就是打两行字。这一课的主角是旁边那份四行的
// CMakeLists.txt：那一串 g++ 参数，从此由它替你写。

#include <iostream>

int main() {
    std::cout << "这份程序是 cmake 编译得到的。" << std::endl;
    std::cout << "四行 CMakeLists.txt，替换了手动输入的那串参数。" << std::endl;
    return 0;
}
