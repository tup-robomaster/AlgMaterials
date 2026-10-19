// 练习 1-8：声明了，但从来没兑现——一个文件报出「未定义的引用」。
//
// 这一份是「错题」：编译那一步（-c）一声不吭地过，链接那一步才炸。
// 页面上那条 undefined reference 是拿这份文件真跑出来的。
//
// 关键的一句：报错是链接器说的，说明语法一个字都没错。
// 它要的东西（twice）白纸黑字写在引号里，缺的就是「兑现」。

#include <iostream>

int twice(int x);

int main() {
    std::cout << twice(21) << std::endl;
    return 0;
}
