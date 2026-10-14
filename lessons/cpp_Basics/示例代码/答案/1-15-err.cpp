// 练习 1-15（找错）—— 引号用成全角的了
//
// 这份故意是错的，编译不过。留着让你见一次真实的样子。

#include <iostream>

int main() {
    int a = 3;
    int b = 5;

    std::cout << “和是” << a + b << std::endl;

    return 0;
}
