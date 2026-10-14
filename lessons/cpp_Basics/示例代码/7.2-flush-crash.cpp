// 7.2 打印调试 —— 程序崩了，用 "\n" 打的那句为什么不见了。
//
// 这一份故意崩（空指针）。照着这样跑，把输出存进文件：
//
//     g++ -std=c++17 -Wall -o flush 7.2-flush-crash.cpp
//     ./flush > out.txt
//     cat out.txt
//
// 存进去的只有「endl 这句」。第二句用 "\n" 打的还在缓冲区里，
// 程序一崩，连缓冲一起没了。1.4 说过 endl 会「催一下」，
// 平时感觉不到——崩掉的这一次就感觉到了。

#include <iostream>

int main() {
    int* p = nullptr;

    std::cout << "endl 这句" << std::endl;
    std::cout << "换行符这句" << "\n";

    std::cout << *p << std::endl;

    return 0;
}
