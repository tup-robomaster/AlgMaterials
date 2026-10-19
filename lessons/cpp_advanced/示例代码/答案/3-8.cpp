// 练习 3-8 的答案。参数写成 const std::vector<int>&：
// 不抄（有 &），也不改（有 const）——求和正好两样都不需要。

#include <iostream>
#include <vector>

int total(const std::vector<int>& v) {
    int sum = 0;

    for (int x : v) {
        sum = sum + x;
    }

    return sum;
}

int main() {
    std::vector<int> v = {90, 85, 77};

    std::cout << total(v) << std::endl;

    return 0;
}
