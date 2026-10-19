// 5.4 错例（故意编不过）：想复制一个 unique_ptr。
//
// 「独占」这两个字是写在类型里的：复制那条路被标准库标成了 = delete，
// 想复制一份，编译器立刻阻止。
// 想换一个持有者，只能移动（std::move）——移动之后原来那个名字就空了。
//
// 编译一下（看报错）：g++ -std=c++17 -Wall 示例代码/5.4-copy-err.cpp -o /tmp/t

#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<int> a = std::make_unique<int>(7);
    std::unique_ptr<int> b = a;     // 想让两个名字都指向它——被阻止在这一行
    return 0;
}
