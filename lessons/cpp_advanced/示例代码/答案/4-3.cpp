// 练习 4-3 的答案：一百万个数的表上，两种找法各要几步。
//
// 4.1 正文那份找数的程序，改两个数就是这一题：表长从 1000 变成 1000000，
// 要找的 1999998 正好在最后一个位置——两种查找方法中最耗时的情况。
// 两个数都是先估算后运行：
//   从头逐个比较：最坏就是遍历整张表，1000000 次，可以直接看出。
//   每次缩小一半：要 2 的几次方才超过 1000000。2^19 = 524288 不够，
//                2^20 = 1048576 够了，所以 20 次。
// 程序里那两个计数器只是验证估算——这一题练的是估算，不是运行。

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;
    for (int i = 0; i < 1000000; i++) {
        v.push_back(i * 2);          // 0 2 4 … 1999998，从小到大排好的
    }

    int target = 1999998;            // 最后一个元素的位置

    int linear = 0;
    for (int i = 0; i < int(v.size()); i++) {
        linear = linear + 1;
        if (v[i] == target) {
            break;
        }
    }

    int steps = 0;
    int lo = 0;
    int hi = int(v.size()) - 1;
    while (lo <= hi) {
        steps = steps + 1;
        int mid = (lo + hi) / 2;
        if (v[mid] == target) {
            break;
        }
        if (v[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    std::cout << "一百万个数里找 " << target << "：" << std::endl;
    std::cout << "  从头逐个比较：" << linear << " 次" << std::endl;
    std::cout << "  每次缩小一半：" << steps << " 次" << std::endl;

    return 0;
}
