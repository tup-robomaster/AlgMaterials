// 练习 4-7 的答案：删掉第 k 个。
//
// 先把要被删的那个数抄下来（gone），再删——删完就找不到它了，
// 而输出里要报「原来是 30」。
// v.begin() + k 就是「第 k 个的位置」：begin() 指着第 0 个，
// 向后移动 k 个位置就是第 k 个。

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

    int k = 0;
    std::cin >> k;

    int gone = v[k];
    v.erase(v.begin() + k);

    std::cout << "删掉第 " << k << " 个（原来是 " << gone << "）之后：";
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
