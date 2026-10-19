// 6.3：除数为 0 时抛出异常，由调用处捕获。
#include <iostream>
#include <stdexcept>

double divide(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("除数不能为 0");
    }
    return a / double(b);
}

int main() {
    int a = 10;
    int b = 0;

    try {
        double result = divide(a, b);
        std::cout << "结果 " << result << std::endl;
    } catch (const std::exception& e) {
        std::cout << "出错：" << e.what() << std::endl;
    }

    std::cout << "程序继续执行" << std::endl;
    return 0;
}
