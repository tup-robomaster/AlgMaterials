// 练习 7-3 正文里贴的那一份（错）：函数里改了，外面没变。
// 该打「a = 6」，它打「a = 5」。

#include <iostream>

void addOne(int x) {
    x = x + 1;
}

int main() {
    int a = 5;

    addOne(a);

    std::cout << "a = " << a << std::endl;

    return 0;
}
