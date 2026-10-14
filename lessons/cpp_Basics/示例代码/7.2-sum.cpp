// 7.2 打印调试 —— 病根找到之后，删掉打印、改掉那一行。
//
// 删的是两句打印，改的是循环里那句 sum = 0;（整行拿掉）。

#include <iostream>

int main() {
    int a[4] = {3, 5, 2, 4};
    int sum = 0;

    for (int i = 0; i < 4; i++) {
        sum = sum + a[i];
    }

    std::cout << "总和：" << sum << std::endl;

    return 0;
}
