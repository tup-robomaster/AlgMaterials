// 3.3：一个变量的命有多长。
//
// local 是在 show 里面声明的，它就活在 show 的那对大括号里。
// 大括号一关，local 就没了——不是「值变成 0」，是这个盒子不存在了。
//
// 这一份就是课上那个动画跑的程序。
//
// 跑一下：g++ -std=c++17 -Wall 示例代码/3.3-scope.cpp -o /tmp/t && /tmp/t

#include <iostream>

void show() {
    int local = 7;
    std::cout << "show 里 local = " << local << std::endl;
}

int main() {
    int a = 3;
    show();
    std::cout << "main 里 a = " << a << std::endl;
    return 0;
}
