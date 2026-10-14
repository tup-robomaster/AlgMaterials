// 3.3：不只是函数。随便一对大括号都圈出一块地盘。
//
// 函数的大括号是一种，if / for / while 的大括号是，光秃秃一对大括号也是。
// 规矩只有一条：在大括号里面声明的盒子，出了这对大括号就不存在了。

#include <iostream>

int main() {
    int a = 1;

    {
        int b = 2;
        std::cout << "大括号里面：a = " << a << "，b = " << b << std::endl;
    }

    std::cout << "大括号外面：a = " << a << std::endl;

    return 0;
}
