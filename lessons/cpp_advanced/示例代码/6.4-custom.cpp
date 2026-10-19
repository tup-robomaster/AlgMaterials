// 6.4：自己定义异常类型，把出错的文件名一起带出来。
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class FileError : public std::runtime_error {
public:
    FileError(const std::string& filename, const std::string& reason)
        : std::runtime_error(filename + "：" + reason) {}
};

void openOrThrow(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        throw FileError(filename, "无法打开");
    }
    std::cout << "打开成功" << std::endl;
}

int main() {
    try {
        openOrThrow("6.4-missing.txt");
    } catch (const std::exception& e) {
        std::cout << "捕获：" << e.what() << std::endl;
    }

    return 0;
}
