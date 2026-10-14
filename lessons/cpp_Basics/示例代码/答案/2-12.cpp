// 练习 2-12（找错）：想打 1 到 5，结果只打到 4 就停了。
//
// 编译一声没吭，跑起来看着也像那么回事——差的那一个数得自己数出来。

#include <iostream>

int main() {
    int i = 1;

    while (i < 5) {
        std::cout << i << std::endl;
        i = i + 1;
    }

    return 0;
}
