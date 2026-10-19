// 练习 6-3：先读一个数，再逐行读后面的内容。
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream in("6-3-in.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }

    int n = 0;
    in >> n;
    in.ignore();

    for (int i = 1; i <= n; i++) {
        std::string line;
        std::getline(in, line);
        std::cout << i << ": " << line << std::endl;
    }

    return 0;
}
