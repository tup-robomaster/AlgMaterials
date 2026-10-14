// 6.3 例题：找到就清成 0，没找到就说一声。
//
// found 一开始是空指针（先写清楚，6.3 那条规矩）。
// 找到就把那个盒子的地址装进去，然后 *found = 0 改的是那个盒子本人。
// 一个都没找到的话，found 还是空的，得先问一句再用。

#include <iostream>

int main() {
    int a = 3;
    int b = 5;

    int want = 5;
    int* found = nullptr;           // 还没找到

    if (a == want) {
        found = &a;
    } else if (b == want) {
        found = &b;
    }

    if (found == nullptr) {
        std::cout << "没找到 " << want << std::endl;
    } else {
        *found = 0;                 // 改的是 found 指着的那个人
        std::cout << "找到 " << want << " 了，已经清成 0" << std::endl;
        std::cout << "a = " << a << "   b = " << b << std::endl;
    }

    return 0;
}
