// 3.2 的第一个函数：把「打一个 n 行的星号三角形」整件事包起来。
//
// 从这儿开始，一个 .cpp 里有两件事：上面那个函数说「怎么打」，
// 下面的 main 说「打哪几个」。这就是「拆开」的样子。

#include <iostream>

void printTriangle(int n) {
    for (int i = 1; i <= n; i = i + 1) {
        for (int j = 1; j <= i; j = j + 1) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }
}

int main() {
    printTriangle(2);
    std::cout << "----" << std::endl;
    printTriangle(4);
    return 0;
}
