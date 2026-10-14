// 5.5 版本二 —— 数据也住进盒子里
//
// 题目没规定 fun 收什么参数，所以「把数组和个数存进盒子」也是照着
// 题目能做出来的一种装法。这一份里 read() / print() / fun()
// 都一个参数不用收——它们是盒子的动作，a 和 n 就在自己家里，
// 直接看得见。
//
// 好处看 main 就知道：版本一里打印那一段写了两遍，
// 这里成了 m.print(); m.fun(); m.print();

#include <iostream>

class MyClass {
private:
    int a[100];
    int n;

public:
    MyClass() {
        n = 0;
    }

    void read() {
        std::cin >> n;

        for (int i = 0; i < n; i++) {
            std::cin >> a[i];
        }
    }

    void fun() {
        int minIdx = 0;
        for (int i = 1; i < n; i++) {
            if (a[i] < a[minIdx]) {
                minIdx = i;
            }
        }

        int t = a[minIdx];
        a[minIdx] = a[n - 1];
        a[n - 1] = t;
    }

    void print() {
        std::cout << a[0];
        for (int i = 1; i < n; i++) {
            std::cout << " " << a[i];
        }
        std::cout << std::endl;
    }
};

int main() {
    MyClass m;

    m.read();
    m.print();
    m.fun();
    m.print();

    return 0;
}
