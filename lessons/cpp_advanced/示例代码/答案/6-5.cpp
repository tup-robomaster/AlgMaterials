// 练习 6-5：读数据算摘要，写进文件，再读回来。
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream in("6-5-in.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }

    int n = 0;
    int sum = 0;
    int x = 0;
    while (in >> x) {
        n++;
        sum = sum + x;
    }
    in.close();

    std::ofstream out("6-5-out.txt");
    if (!out) {
        std::cout << "文件创建失败" << std::endl;
        return 1;
    }
    out << "个数 " << n << std::endl;
    out << "总和 " << sum << std::endl;
    out << "平均 " << sum / n << std::endl;
    out.close();

    std::ifstream back("6-5-out.txt");
    if (!back) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(back, line)) {
        std::cout << "读回：" << line << std::endl;
    }
    back.close();

    std::remove("6-5-out.txt");
    return 0;
}
