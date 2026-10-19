// 4.3 例题：往已经排好序的一组数里插入一个，插入后仍然有序。
//
// 找位置：从前往后逐个比较，遇到第一个比新数大的位置停下，新数就插在它前面。
// 比较到末尾都没停，说明新数最大——那时候 it 正好是 end()，
// 插在 end() 前面就是插在最后。end() 也能当位置用，这是头一回见。
//
// 输入：第一行 n，第二行 n 个从小到大排好的数，第三行要插进来的那个。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.3-insert-sorted.cpp -o /tmp/t && /tmp/t < 示例代码/4.3-insert-sorted.in

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

    int fresh = 0;
    std::cin >> fresh;

    auto it = v.begin();
    while (it != v.end() && *it < fresh) {
        ++it;
    }

    v.insert(it, fresh);

    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
