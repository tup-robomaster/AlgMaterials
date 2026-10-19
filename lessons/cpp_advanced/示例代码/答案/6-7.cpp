// 练习 6-7：除数为 0 时抛出异常，主函数捕获后继续读下一组。
#include <iostream>
#include <stdexcept>

int safeDivide(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("除数为 0");
    }
    return a / b;
}

int main() {
    int a = 0;
    int b = 0;
    while (std::cin >> a >> b) {
        try {
            int result = safeDivide(a, b);
            std::cout << a << " / " << b << " = " << result << std::endl;
        } catch (const std::runtime_error& e) {
            std::cout << "出错：" << e.what() << std::endl;
        }
    }
    return 0;
}
