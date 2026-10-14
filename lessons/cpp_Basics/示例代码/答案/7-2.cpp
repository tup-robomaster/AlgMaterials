// 练习 7-2（找错）：最小值从 0 开始找。
//
// 病根那一行是 int mn = 0;——0 比这五个数都小，
// if 里的条件一次都没成立过，mn 从头到尾没人换得掉，最后打出来就是 0。
// 改成 int mn = a[0]; 就打 3。

#include <iostream>

int main() {
    int a[5] = {7, 3, 9, 4, 6};
    int mn = a[0];

    for (int i = 0; i < 5; i++) {
        if (a[i] < mn) {
            mn = a[i];
        }
    }

    std::cout << "最小值：" << mn << std::endl;

    return 0;
}
