// 4.6 例题：4.5 那道综合练习，用 vector 再写一遍。
//
// 和 4.5-min-to-last.cpp 摆在一起看，差别就在头上那几行：
//
//   数组版                       vector 版
//   int a[100];                  std::vector<int> a;
//   int n; std::cin >> n;        int n; std::cin >> n;
//   for (...) std::cin >> a[i];  for (...) a.push_back(x);
//   ...a[n-1]...                 ...a[a.size()-1]...
//
// 找最小、交换、那两段打印的逻辑一个字没改。
// 那个「开多大」的问题没有了——不用猜，也不用将就。

#include <iostream>
#include <vector>

int main() {
    int n;
    std::cin >> n;

    std::vector<int> a;
    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        a.push_back(x);
    }

    int minIdx = 0;
    for (int i = 1; i < int(a.size()); i++) {
        if (a[i] < a[minIdx]) {
            minIdx = i;
        }
    }

    int last = int(a.size()) - 1;

    // 交换前
    std::cout << a[0];
    for (int i = 1; i < int(a.size()); i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    int t = a[minIdx];
    a[minIdx] = a[last];
    a[last] = t;

    // 交换后
    std::cout << a[0];
    for (int i = 1; i < int(a.size()); i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    return 0;
}
