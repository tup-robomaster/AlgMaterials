// 3.3：const 也写在变量本身上——「这个数，从今往后不再改变」。
//
// 基础课没见过这种写法：那儿的 const 只出现在参数表上（const std::string&）。
// 这一份说的是前面那个 const：变量自身决定不再改变。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.3-const-var.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    const int N = 5;
    const double PI = 3.14159;

    std::vector<int> v(N);              // 要 N 个空位

    for (int i = 0; i < N; i++) {
        v[i] = i * i;
    }

    for (int i = 0; i < N; i++) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;

    std::cout << PI * 2 << std::endl;

    return 0;
}
