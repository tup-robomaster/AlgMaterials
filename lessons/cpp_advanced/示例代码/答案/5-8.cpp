// 练习 5-8 的答案：练习 5-1 那件事，用新工具重写一遍。
//
// 两处不同，一处不变：
//   不同的——申请内存那行没写 new，释放内存那行没写 delete；
//   不变的是用法——data[i] 照写，跟普通数组一模一样。
// 申请和释放都交给了 data 这个对象：它一销毁，内存随之释放。

#include <iostream>
#include <memory>

int main() {
    int n = 0;
    std::cin >> n;

    std::unique_ptr<int[]> data = std::make_unique<int[]>(n);
    for (int i = 0; i < n; i++) {
        std::cin >> data[i];
    }

    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum = sum + data[i];
    }

    std::cout << n << " 个数的和是 " << sum << std::endl;

    return 0;
}
