// 练习 8-1 答案：读两个整数夹一个符号，算出来。
//
// 符号用 char 装：std::cin 读一个字符，读到的是那个符号本身。
// 所以 op == '+' 就是「这个字符是不是加号」。
//
// 除以 0 那一支要写在算之前——不然 a / b 先崩给你看，
//「除数不能是 0」那句话根本轮不到打（1.3 讲过除以 0 是什么下场）。

#include <iostream>

int calc(int a, char op, int b) {
    if (op == '+') {
        return a + b;
    } else if (op == '-') {
        return a - b;
    } else if (op == '*') {
        return a * b;
    } else {
        return a / b;
    }
}

int main() {
    int a;
    int b;
    char op;

    std::cin >> a >> op >> b;

    if (op == '/' && b == 0) {
        std::cout << "除数不能是 0" << std::endl;
        return 0;
    }

    std::cout << a << " " << op << " " << b
              << " = " << calc(a, op, b) << std::endl;

    return 0;
}
