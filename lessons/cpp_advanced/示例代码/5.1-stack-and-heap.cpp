// 5.1 例题：两处存储位置——一个变量在栈上，一块内存在堆上。
//
// 打印地址不是为了记住某个数，是为了确认这两个变量存放在不同的位置：
// 栈上那个的地址和 &onHeap 很接近，堆上那个的地址离它们很远。
// 每次运行时地址都不一样，记格式，不要记具体的数字。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.1-stack-and-heap.cpp -o /tmp/t && /tmp/t

#include <iostream>

int main() {
    int on_stack = 42;              // 栈上：进 main 就分配好，出 main 自动释放
    int* on_heap = new int;         // 堆上：得用 new 申请，它才划出一块内存
    *on_heap = 42;                  // 那块内存没有名字，通过指针写进去

    std::cout << "栈上的变量：值 " << on_stack << "，它存放在 " << &on_stack
              << std::endl;
    std::cout << "堆上的内存：值 " << *on_heap << "，它在 " << on_heap
              << std::endl;
    std::cout << "保存那个地址的指针，存放在 " << &on_heap << std::endl;
    std::cout << "——前两个地址离得远，后两个离得近。" << std::endl;

    delete on_heap;                 // 申请与释放对应：这块内存释放回堆上
    on_heap = nullptr;              // 释放后把指针置成空

    return 0;
}
