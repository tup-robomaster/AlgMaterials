// 练习 3-3 的答案。要改到外面那个，参数得是引用（多一个 &）；
// 函数中每个数乘 2，依靠的是范围 for 里的那个 &。

#include <iostream>
#include <vector>

void doubleAll(std::vector<int>& v) {
    for (int& x : v) {
        x = x * 2;
    }
}

int main() {
    std::vector<int> v = {3, 1, 4};

    doubleAll(v);

    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
