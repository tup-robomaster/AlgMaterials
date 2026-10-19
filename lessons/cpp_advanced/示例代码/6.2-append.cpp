// 6.2：用 std::ios::app 追加写入，两次打开都不会覆盖前面的内容。
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::remove("6.2-log.txt");

    std::ofstream first("6.2-log.txt", std::ios::app);
    first << "第一次写入" << std::endl;
    first.close();

    std::ofstream second("6.2-log.txt", std::ios::app);
    second << "第二次写入" << std::endl;
    second.close();

    std::ifstream in("6.2-log.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << std::endl;
    }

    std::remove("6.2-log.txt");
    return 0;
}
