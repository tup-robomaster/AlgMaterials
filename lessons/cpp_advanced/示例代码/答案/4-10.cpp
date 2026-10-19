// 练习 4-10 的答案：在每个奇数前面插一个 0。
//
// 形状还是「一边走一边动」，只是这次是插不是删。
// insert 跟 erase 一样，把位置还给你——还的是「刚插进去那个 0」的位置。
// 插完不能直接 ++it：那样下一个看的就是原来那个奇数，
// 它还是奇数，又会再插一个 0，没完没了。
// 要跨两格：跨过刚插的 0，再跨过原来那个奇数，it 才落到下一个该看的数上。

#include <iostream>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> v;
    for (int i = 0; i < n; i++) {
        int x = 0;
        std::cin >> x;
        v.push_back(x);
    }

    for (auto it = v.begin(); it != v.end();) {
        if (*it % 2 != 0) {
            it = v.insert(it, 0);
            it = it + 2;
        } else {
            ++it;
        }
    }

    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
