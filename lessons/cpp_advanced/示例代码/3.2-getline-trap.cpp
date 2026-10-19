// 3.2：>> 后面紧跟 getline，第一次读到的是一行空的。
//
// 这是「读一整行」这条路上一块必踩的石头，不是编译器的问题，
// 是 >> 读到数字就停下了、回车还留在输入里。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.2-getline-trap.cpp -o /tmp/t &&
//         /tmp/t < 示例代码/3.2-getline-trap.in

#include <iostream>
#include <string>

int main() {
    int n;
    std::cin >> n;              // 读完 4 就停了，后面那个回车没动

    for (int i = 0; i < n; i++) {
        std::string line;
        std::getline(std::cin, line);   // 第一圈读到的是那个回车

        std::cout << i + 1 << ": [" << line << "]" << std::endl;
    }

    return 0;
}
