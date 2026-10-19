// 练习 6-1：从文件里读 n 和 n 个数，求和。
#include <fstream>
#include <iostream>

int main() {
    std::ifstream in("6-1-in.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }

    int n = 0;
    in >> n;

    int sum = 0;
    for (int i = 0; i < n; i++) {
        int x = 0;
        in >> x;
        sum = sum + x;
    }

    std::cout << n << " 个数的和是 " << sum << std::endl;
    return 0;
}
