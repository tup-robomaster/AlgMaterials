// 练习 0-1 的答案：读两个整数，打印和。
// 用的是基础课 1.5 那套 cin——两个数分两次读，中间不用管空格还是回车。

#include <iostream>

int main() {
    int a = 0;
    int b = 0;
    std::cin >> a >> b;
    std::cout << a + b << std::endl;
    return 0;
}
