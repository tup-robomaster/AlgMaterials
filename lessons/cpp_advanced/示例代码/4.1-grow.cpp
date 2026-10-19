// 4.1：n 翻一倍，步数翻几倍？
//
// 上一步数出来的 5 和 25 看着差不多，是因为 5 太小。
// 让 n 一千一千地翻上去，两个循环都实际运行一遍，把次数输出并排比较：
// 一层那个跟着 n 翻倍，两层那个翻的是四倍。
//
// 这张表就是「时间复杂度」这一术语要表达的内容——不看具体的数，
// 看的是「n 变大时它照着什么长」。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.1-grow.cpp -o /tmp/t && /tmp/t

#include <iostream>

int main() {
    for (int n = 1000; n <= 8000; n = n * 2) {
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

        std::cout << "n = " << n << "：一层 " << one
                  << " 步，两层 " << two << " 步" << std::endl;
    }

    return 0;
}
