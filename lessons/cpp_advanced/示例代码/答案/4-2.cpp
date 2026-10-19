// 练习 4-2 的答案：三段代码「跑几次」的实测。
//
// 估的时候不用写程序——三段都是数出来的：
//   一：三层循环，外圈转 n 次，每转一次中圈转 n 次，每转一次里圈转 n 次，
//       乘起来 n³ = 1000000000 次。
//   二：两个循环并排，不是套着，加起来 n + 5 = 1005 次。
//   三：每转一圈 x 砍一半，转过几圈就是「1000 砍几次到 1」——
//       2^9 = 512、2^10 = 1024，所以是 9 圈，也就是 log₂n 取整。
// 下面把三段真跑一遍，和估的数对一对。第三段那个 x = x / 2
// 就是 4.1 那个「砍一半」的另外一种写法。

#include <iostream>

int main() {
    const int n = 1000;

    long long one = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                one = one + 1;
            }
        }
    }

    int two = 0;
    for (int i = 0; i < n; i++) {
        two = two + 1;
    }
    for (int j = 0; j < 5; j++) {
        two = two + 1;
    }

    int three = 0;
    int x = n;
    while (x > 1) {
        x = x / 2;
        three = three + 1;
    }

    std::cout << "一：" << one << " 次（n 的三次方）" << std::endl;
    std::cout << "二：" << two << " 次（n 加 5）" << std::endl;
    std::cout << "三：" << three << " 次（log2n 取整）" << std::endl;

    return 0;
}
