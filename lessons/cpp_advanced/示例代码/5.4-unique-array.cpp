// 5.4 例题：一整块连续内存也有现成的写法——make_unique<int[]>(n)。
//
// 和手写 new int[n] / delete[] 的规则一样：谁申请谁释放。
// 只是「释放」现在写在类型里，不用你在每条出口上记着写。
// 用起来跟数组一样，data[i] 照写。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.4-unique-array.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <memory>

int main() {
    std::unique_ptr<int[]> data = std::make_unique<int[]>(5);

    for (int i = 0; i < 5; i++) {
        data[i] = (i + 1) * 10;
    }

    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum = sum + data[i];
    }

    std::cout << "加起来：" << sum << std::endl;
    std::cout << "最后一个：" << data[4] << std::endl;

    return 0;
}
