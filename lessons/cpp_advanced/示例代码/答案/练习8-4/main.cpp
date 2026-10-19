#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

void read_config(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("打不开配置文件");
    }

    std::string line;
    while (std::getline(in, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) {
            throw std::runtime_error("配置行格式错误");
        }
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        std::cout << key << " = " << value << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::string path = "config.txt";
    if (argc > 1) {
        path = argv[1];
    }

    try {
        read_config(path);
    } catch (const std::exception& e) {
        std::cout << "配置读取失败：" << e.what() << "\n";
    }
    return 0;
}
