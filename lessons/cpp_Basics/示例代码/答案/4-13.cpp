// 练习 4-13 的答案：用 3.5 的 swapTwo 交换数组的首尾。
//
// 函数体一个字没改，还是 3.5 那三行——这次传进去的是两个数组元素。
// 参数上的 & 是关键：没有它，换的只是函数里的两个副本，外面不动。
//
// 用函数没有让程序变短（这里只调用一次）。3.4 讲过理由：
// 名字。swapTwo 这三个字一读就知道在干什么，
// 而三行裸赋值要读两遍才敢确定。

#include <iostream>

void swapTwo(int& x, int& y) {
    int t = x;
    x = y;
    y = t;
}

int main() {
    int a[5] = {1, 2, 3, 4, 5};

    swapTwo(a[0], a[4]);

    std::cout << a[0];
    for (int i = 1; i < 5; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    return 0;
}
