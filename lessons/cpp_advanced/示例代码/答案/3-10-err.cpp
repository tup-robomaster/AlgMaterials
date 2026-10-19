// 练习 3-10 的答案。上一题的 Counter 一个字没改，只在 main 里
// 多写了 const——它无法编译，而且本就不应编译通过。
//
// 报错是拿这份文件真跑出来的，页面上抄的那几行和它逐字对得上。

#include <iostream>

class Counter {
public:
    int n;

    Counter() {
        n = 0;
    }

    void add(int k) {
        n = n + k;
    }

    int get() const {
        return n;
    }
};

int main() {
    const Counter c;

    c.add(3);

    std::cout << c.get() << std::endl;

    return 0;
}
