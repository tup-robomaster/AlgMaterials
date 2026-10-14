// 练习 0-4 的答案：把「第三行」和「第一行」那两句整句对调。
//
// 对调的是整整两行——从 std::cout 到分号，一个字符不漏地换位置。

#include <iostream>

int main() {
    std::cout << "第三行" << std::endl;
    std::cout << "第二行" << std::endl;
    std::cout << "第一行" << std::endl;
    return 0;
}
