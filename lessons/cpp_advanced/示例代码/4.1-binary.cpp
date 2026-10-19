// 4.1：第三种循环——每次循环，需要检查的范围缩小一半。
//
// 一千个排好序的数里查找一个数，从头逐个比较最坏要一千次；
// 每次都从中间分开、只检查剩余的一半，十次就够了。
// 这就是 3.4 里 map 按名字就能直接取值的依据：它内部的数据是有序的。
//
// 前提需要说明：这些数必须是有序的。无序的表里无法「缩小一半」——
// 你无法确定目标在哪一半。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.1-binary.cpp -o /tmp/t && echo 1998 | /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;
    for (int i = 0; i < 1000; i++) {
        v.push_back(i * 2);          // 0 2 4 … 1998，从小到大排好的
    }

    int target = 0;
    std::cin >> target;

    int linear = 0;
    int linear_at = -1;
    for (int i = 0; i < int(v.size()); i++) {
        linear = linear + 1;
        if (v[i] == target) {
            linear_at = i;
            break;
        }
    }

    int steps = 0;
    int lo = 0;
    int hi = int(v.size()) - 1;
    int pos = -1;
    while (lo <= hi) {
        steps = steps + 1;
        int mid = (lo + hi) / 2;
        if (v[mid] == target) {
            pos = mid;
            break;
        }
        if (v[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }

    std::cout << "一千个数里找 " << target << "：" << std::endl;
    std::cout << "  从头逐个比较：" << linear << " 次";
    if (linear_at >= 0) {
        std::cout << "，在第 " << linear_at << " 个";
    }
    std::cout << std::endl;
    std::cout << "  每次缩小一半：" << steps << " 次";
    if (pos >= 0) {
        std::cout << "，在第 " << pos << " 个";
    }
    std::cout << std::endl;

    return 0;
}
