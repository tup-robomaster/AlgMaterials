// 练习 4-5 的答案：遍历求和。
//
// sum 开在循环外面——开在里面的话每转一圈就清零，
// 最后打出来的只是最后一个元素。

#include <iostream>

int main() {
    int a[5] = {7, 0, 5, 2, 9};

    int sum = 0;

    for (int i = 0; i < 5; i++) {
        sum = sum + a[i];
    }

    std::cout << sum << std::endl;

    return 0;
}
