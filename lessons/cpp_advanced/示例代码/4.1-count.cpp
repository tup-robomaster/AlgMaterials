// 4.1：同一份数据，两种数法——「估快不快」的全部做法就是数这一下。
//
// 两个循环干的事一模一样：把 count 加 1。差的是套了几层。
// 一层那个执行 n 次，两层那个执行 n² 次。实际统计出来，比背「O(n²)」更有效。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.1-count.cpp -o /tmp/t && echo 5 | /tmp/t

#include <iostream>

int main() {
    int n = 0;
    std::cin >> n;

    int one = 0;
    for (int i = 0; i < n; i++) {
        one = one + 1;
    }

    int two = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            two = two + 1;
        }
    }

    std::cout << "一层：" << one << " 次" << std::endl;
    std::cout << "两层：" << two << " 次" << std::endl;

    return 0;
}
