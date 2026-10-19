// 3.3：写在变量上的 const，修改它就是这种结果。
//
// 跟 3.3-modify-err.cpp 的报错长得像，但这句话说的是 variable 不是 reference，
// 位置上也不一样：一个是赋值的时候，一个是调函数的时候。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.3-const-var-err.cpp -o /tmp/t

#include <iostream>

int main() {
    const int N = 5;

    N = 6;

    std::cout << N << std::endl;

    return 0;
}
