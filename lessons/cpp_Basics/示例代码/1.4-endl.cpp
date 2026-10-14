// 1.4 补充：换行的两种写法
//
// std::endl 和 "\n" 都能换行。这个程序三行都用上了，跑出来还是三行。

#include <iostream>

int main() {
    std::cout << "第一行" << std::endl;
    std::cout << "第二行" << "\n";
    std::cout << "第三行" << std::endl;

    return 0;
}
