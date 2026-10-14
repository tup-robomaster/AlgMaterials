// 7.2 打印调试 —— 上面那份一个字没改，只在循环里加了一句打印。
//
// 打印加在「加之前」和「加完」两头，这样一轮的进和出都在眼前。
// 跑出来一看就知道：sum 每轮都被清零了。

#include <iostream>

int main() {
    int a[4] = {3, 5, 2, 4};
    int sum = 0;

    for (int i = 0; i < 4; i++) {
        std::cout << "第 " << i << " 轮：加之前 sum = " << sum;
        sum = 0;
        sum = sum + a[i];
        std::cout << "，加完 " << a[i] << " 之后 sum = " << sum << std::endl;
    }

    std::cout << "总和：" << sum << std::endl;

    return 0;
}
