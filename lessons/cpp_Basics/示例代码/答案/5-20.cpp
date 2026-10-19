// 练习 5-20 的答案（找错）：只改一个数。
//
// 病根在 int minIdx = 1; —— 第一个格子被排除在候选之外了。
// 它本来该是 0：先认定第 0 个最小，再拿后面每一个去挑战它。
//
// 为什么题目那组样例看不出来：5 34 1 2 56 4 里最小值 1 正好在编号 1 上，
// 而错的那份就是从编号 1 开始找的，正好对上了。
// 换一组让最小值落在第 0 个的（5 1 9 3 7 4），它当场露馅。
//
// 这也是这一类错最坑的地方：样例过了不等于对。
//
// 输入在 5-20.in 里：5 1 9 3 7 4

#include <iostream>

class MyClass {
public:
    void fun(int a[], int n) {
        int minIdx = 0;

        for (int i = 1; i < n; i++) {
            if (a[i] < a[minIdx]) {
                minIdx = i;
            }
        }

        int t = a[minIdx];
        a[minIdx] = a[n - 1];
        a[n - 1] = t;
    }
};

int main() {
    int a[100];
    int n;

    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    // 交换前
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    MyClass m;
    m.fun(a, n);

    // 交换后
    std::cout << a[0];
    for (int i = 1; i < n; i++) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    return 0;
}
