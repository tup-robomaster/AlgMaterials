// 1.3 运算 —— 这一课最重要的一个例子
//
// 同样是 7 除以 2，四行的写法只差一个小数点，结果却不一样。
// 先自己猜四行分别输出什么，再跑。

#include <iostream>

int main() {
    std::cout << 7 / 2 << std::endl;       // 整数 除以 整数
    std::cout << 7.0 / 2 << std::endl;     // 分子写成小数
    std::cout << 7 / 2.0 << std::endl;     // 分母写成小数
    std::cout << 7.0 / 2.0 << std::endl;   // 两个都写成小数

    return 0;
}
