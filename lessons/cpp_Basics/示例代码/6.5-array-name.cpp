// 6.5 数组名几乎就是首元素的地址
//
// a 和 &a[0] 是同一个地址——数组名用起来就是那一排的第一个门牌号。
// 但 sizeof 上能看出它俩不一样：见课里那段。

#include <iostream>

int main() {
    int a[3] = {10, 20, 30};

    std::cout << "数组名和首元素地址一样吗  " << (a == &a[0]) << std::endl;
    std::cout << "整个数组占几格  " << sizeof(a) << std::endl;

    return 0;
}
