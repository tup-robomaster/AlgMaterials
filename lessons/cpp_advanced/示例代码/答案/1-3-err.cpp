// 练习 1-3：同一个函数写两遍，看编译器怎么翻脸。
//
// 这一份是「错题」——它编不过，而且就该编不过。
// 页面上的报错是拿这份文件真跑出来的。
//
// 报错对应课本 1.2：一个文件里贴了两遍定义。

#include <iostream>

int twice(int x) {
    return x * 2;
}

int twice(int x) {
    return x * 2;
}

int main() {
    std::cout << twice(21) << std::endl;
    return 0;
}
