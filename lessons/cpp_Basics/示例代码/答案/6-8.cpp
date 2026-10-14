// 练习 6-8 的答案：用之前先问一句
//
// 先判空再解引用——顺序反过来，第二次调用就会当场崩（6.3 那份 -crash）。

#include <iostream>

void print(int* p) {
    if (p == nullptr) {
        std::cout << "空" << std::endl;
        return;
    }

    std::cout << *p << std::endl;
}

int main() {
    int a = 5;

    print(&a);
    print(nullptr);

    return 0;
}
