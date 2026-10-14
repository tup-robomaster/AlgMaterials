// 练习 4-7 的答案：编号和值一起打。
//
// i 在一行里用了两次：一次当编号打出来，一次当方括号里的下标。

#include <iostream>

int main() {
    int a[5] = {7, 0, 5, 2, 9};

    for (int i = 0; i < 5; i++) {
        std::cout << i << ": " << a[i] << std::endl;
    }

    return 0;
}
