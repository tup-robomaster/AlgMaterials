// 练习 3-2 的答案。倒序输出依靠下标，不依靠范围 for——
// 范围 for 只会从前往后一趟走完，倒着来得自己数。

#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;

    std::vector<int> v;

    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        v.push_back(x);
    }

    for (int i = n - 1; i >= 0; i--) {
        std::cout << v[i] << std::endl;
    }

    return 0;
}
