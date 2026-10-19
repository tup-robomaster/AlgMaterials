// 练习 3-14 的答案。去重和排序 set 都替你做了：
// 重复插入的元素会被忽略，遍历输出的顺序本身从小到大。

#include <iostream>
#include <set>

int main() {
    int n;
    std::cin >> n;

    std::set<int> s;

    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;

        s.insert(x);
    }

    std::cout << "一共 " << s.size() << " 种" << std::endl;

    for (int x : s) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
