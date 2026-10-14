// 1.4 补充：引号里的和引号外的
//
// 三行几乎一样，只有引号的位置在挪。这是 cout 最要紧的一条规矩。

#include <iostream>

int main() {
    std::cout << "3+5" << std::endl;            // 引号里：原样打
    std::cout << 3 + 5 << std::endl;            // 引号外：先算，打结果
    std::cout << "3+5=" << 3 + 5 << std::endl;  // 一半一半

    return 0;
}
