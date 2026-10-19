#include <iostream>

int main() {
    int* p = nullptr;
    std::cout << (p == nullptr ? "空着" : "指着东西") << std::endl;
    return 0;
}
