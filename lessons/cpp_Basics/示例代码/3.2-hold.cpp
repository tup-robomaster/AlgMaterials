// 3.2：return 交回来的东西，可以像普通变量一样接住。
//
// 和 3.2-return.cpp 是同一个 doubleIt，只是这次 main 里
// 先把它接住、再加 1 打出来——交回来的值就是个普通的数。

#include <iostream>

int doubleIt(int x) {
    return x * 2;
}

int main() {
    int result = doubleIt(3);
    std::cout << result + 1 << std::endl;
    return 0;
}
