// 6.4 三种摆在一起，同一个 a 连着过三道手
//
// 函数体里那句「加一」三个版本长得几乎一样，
// 变的只有参数表上那一个符号。

#include <iostream>

void byValue(int x)     { x = x + 1; }
void byPointer(int* p)  { *p = *p + 1; }
void byRef(int& x)      { x = x + 1; }

int main() {
    int a = 3;

    byValue(a);
    std::cout << "值传递之后    " << a << std::endl;

    byPointer(&a);
    std::cout << "地址传递之后  " << a << std::endl;

    byRef(a);
    std::cout << "引用传递之后  " << a << std::endl;

    return 0;
}
