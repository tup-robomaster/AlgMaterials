// 3.5：练习 3-10 发下来的那一份，就是下面这样，一个字没改。
//
// 想把传进来的数翻一倍，函数里也确实乘了 2，可 main 里的 a 打出来还是 6。
// 编译器照样一声不吭——跟 3.4 那个 swapTwo 是同一个毛病。
//
// 想不通的话，回去看 3.4：函数参数表上的 int x，是一个**新盒子**，
// 跟 main 里的 a 没有关系。改了 x，等于改了那个新盒子里的复印件。

#include <iostream>

void doubleIt(int x) {
    x = x * 2;
}

int main() {
    int a = 6;
    doubleIt(a);
    std::cout << "现在 a = " << a << std::endl;
    return 0;
}
