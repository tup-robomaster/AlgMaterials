// 练习 7-11（预测输出）：打的是 5 9。
//
// 读的时候盯住 k 这个盒子：它装的是「到目前为止最大那个数的编号」，
// 一发现更大的就把编号换掉。读完了加一句打印，看它是不是这么挪的。

#include <iostream>

int main() {
    int a[6] = {3, 1, 4, 1, 5, 9};
    int k = 0;

    for (int i = 0; i < 6; i++) {
        if (a[i] > a[k]) {
            k = i;
        }
        std::cout << "i = " << i << " 时，k = " << k << std::endl;
    }

    std::cout << k << " " << a[k] << std::endl;

    return 0;
}
