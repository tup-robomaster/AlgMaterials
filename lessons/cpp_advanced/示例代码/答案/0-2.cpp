// 练习 0-2 的答案：三个数里最大的那个。
// 拆成一个函数是重点——基础课 3.1 讲的就是「一段活单独拆出去」。
// 也可以写成 if / else if 三层，但函数那份才是这门课以后要用的写法：
// 第 1 部分马上要把这个函数拆到另一个文件里去。

#include <iostream>

int max3(int a, int b, int c) {
    int m = a;
    if (b > m) {
        m = b;
    }
    if (c > m) {
        m = c;
    }
    return m;
}

int main() {
    std::cout << max3(12, 7, 30) << std::endl;
    return 0;
}
