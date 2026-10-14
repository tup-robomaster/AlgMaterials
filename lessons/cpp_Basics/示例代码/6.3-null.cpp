// 6.3 空指针：还没指到任何地方
//
// nullptr 的意思是「我手里还没有门牌号」。一个刚开出来、
// 还没来得及赋值的指针就是这种状态，用它之前先问一句。

#include <iostream>

int main() {
    int* p = nullptr;

    std::cout << "p 是空的吗  " << (p == nullptr) << std::endl;

    if (p == nullptr) {
        std::cout << "它还没指到任何盒子，现在不能用" << std::endl;
    }

    return 0;
}
