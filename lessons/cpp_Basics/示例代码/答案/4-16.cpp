// 练习 4-16 的答案：vector 装三个数，循环打印。
//
// 遍历的写法跟数组一模一样，只有条件那一处多了个 int()——
// 为什么，见 4.6 和练习 4-17 的答案。

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    for (int i = 0; i < int(v.size()); i++) {
        std::cout << v[i] << std::endl;
    }

    return 0;
}
