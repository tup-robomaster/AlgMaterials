// 6.2：写一份文件，关掉之后再打开读回来，确认写进去的内容。
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ofstream out("6.2-report.txt");
    if (!out) {
        std::cout << "文件创建失败" << std::endl;
        return 1;
    }
    out << "总分 414" << std::endl;
    out << "平均 82.8" << std::endl;
    out.close();

    std::ifstream in("6.2-report.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << std::endl;
    }

    std::remove("6.2-report.txt");
    return 0;
}
