// 练习 3-1 的答案。数好五个数写进大括号，剩下的事交给范围 for。

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5};

    for (int x : v) {
        std::cout << x << std::endl;
    }

    return 0;
}
