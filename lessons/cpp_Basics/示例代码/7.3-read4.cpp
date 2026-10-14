// 练习 7-14 正文里贴的那一份：读它，说清那个 18 是怎么来的。
// 这一份没有错，错的是「一眼看快」的那种读法。

#include <iostream>

int main() {
    int a[6] = {2, 4, 6, 8, 10, 12};
    int s = 0;

    for (int i = 0; i < 6; i++) {
        if (i % 2 == 0) {
            s = s + a[i];
        }
    }

    std::cout << s << std::endl;

    return 0;
}
