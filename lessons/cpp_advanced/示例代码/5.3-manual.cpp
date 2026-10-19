// 5.3 例题（对照用）：手动管理的那一版——自己申请一块，算完自己释放。
//
// 函数有两个出口：一个是「中途找到就返回」，一个是「从头执行到尾」。
// delete[] 只写在后面那条路径上——运行两次，一次走这条路径、一次走那条路径，
// 看哪一次没有释放那块内存。
// 这就是手动管理的麻烦：释放的动作要在每条出口上都写一遍，漏一处就泄漏一次。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.3-manual.cpp -o /tmp/t && /tmp/t

#include <iostream>

// 1 到 n 加起来，第一次超过 limit 时用到了几个数。
// 中途就超了，就提前返回；一直没超，就计算到 n。
int count_until_over(int n, int limit) {
    int* data = new int[n];
    for (int i = 0; i < n; i++) {
        data[i] = i + 1;
    }

    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum = sum + data[i];
        if (sum > limit) {
            std::cout << "（中途返回的出口——这里没有写 delete[]）" << std::endl;
            return i + 1;       // 出口一：提前返回，内存没有释放
        }
    }

    std::cout << "（正常结束的出口——这里写了 delete[]）" << std::endl;
    delete[] data;              // 出口二：正常结束，已释放
    return n;
}

int main() {
    std::cout << "限 10：" << count_until_over(10, 10) << " 个数" << std::endl;
    std::cout << "限 100：" << count_until_over(10, 100) << " 个数" << std::endl;
    return 0;
}
