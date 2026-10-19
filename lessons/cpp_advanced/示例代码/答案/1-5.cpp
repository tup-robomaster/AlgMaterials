// 练习 1-5：定义搬到 main 下面，上面只留一句声明。
//
// 这就是「头文件」的缩影：上面那行 int twice(int x); 等于一个
// 只有一行的小头文件。1.3 里的 stats.h 干的是同一件事，只是多几行。
//
// 编译器是自上往下读的：读到 main 里那句 twice(21) 的时候，
// 它得先见过一个叫 twice 的东西，才敢让你调用。
// 至于函数体在哪儿，那是后面（链接）的事。

#include <iostream>

int twice(int x);

int main() {
    std::cout << twice(21) << std::endl;
    return 0;
}

int twice(int x) {
    return x * 2;
}
