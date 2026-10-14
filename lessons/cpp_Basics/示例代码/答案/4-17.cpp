// 练习 4-17 的答案：读 n 个数，求和。
//
// vector 版不用先猜一个够大的数字：读一个塞一个，塞几个就有几个。
// 综合练习二里那句 int a[100]; 的「将就」，到这儿就没有了。

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

    int sum = 0;
    for (int i = 0; i < int(v.size()); i++) {
        sum = sum + v[i];
    }

    std::cout << sum << std::endl;

    return 0;
}
