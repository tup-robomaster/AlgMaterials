// 练习 3-9 的答案。const 写在参数表那个括号后面、大括号前面。
// get 不修改对象中的任何状态，所以它能加 const；add 要修改 n，所以它不能加。

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
    Counter c;

    c.add(3);
    c.add(4);

    std::cout << c.get() << std::endl;

    return 0;
}
