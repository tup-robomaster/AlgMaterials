// 练习 6-4：把 1 到 n 写进文件，再读回来求和。
#include <cstdio>
#include <fstream>
#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    std::ofstream out("6-4-out.txt");
    if (!out) {
        std::cout << "文件创建失败" << std::endl;
        return 1;
    }
    for (int i = 1; i <= n; i++) {
        out << i << std::endl;
    }
    out.close();

    std::ifstream in("6-4-out.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }

    int sum = 0;
    int x = 0;
    while (in >> x) {
        sum = sum + x;
    }

    std::cout << "写入 " << n << " 个数，读回来的和是 " << sum << std::endl;

    std::remove("6-4-out.txt");
    return 0;
}
