// 练习 6-6：把输入逐行追加到日志文件，结束后读回来。
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ofstream out("6-6-log.txt", std::ios::app);
    if (!out) {
        std::cout << "文件打开失败" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "end") {
            break;
        }
        out << line << std::endl;
    }
    out.close();

    std::ifstream in("6-6-log.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }

    std::cout << "日志内容：" << std::endl;
    while (std::getline(in, line)) {
        std::cout << line << std::endl;
    }
    in.close();

    std::remove("6-6-log.txt");
    return 0;
}
