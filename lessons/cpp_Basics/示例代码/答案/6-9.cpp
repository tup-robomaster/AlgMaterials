// 练习 6-9 的答案：换的是指着的值，不是指针自己
//
// 错的那份写的是 x = y;——换的是两个指针盒子里的地址，
// 外面那两个 int 一个都没动。改成 *x / *y，写的才是本人。
//
// 顺带一句：指针不用再传引用了（int*& 那种写法又绕又容易错），
// 因为它本来就能直接够到外面那个盒子。

#include <iostream>

void swapTwo(int* x, int* y) {
    int t = *x;
    *x = *y;
    *y = t;
}

int main() {
    int a = 1;
    int b = 2;

    swapTwo(&a, &b);

    std::cout << a << " " << b << std::endl;

    return 0;
}
