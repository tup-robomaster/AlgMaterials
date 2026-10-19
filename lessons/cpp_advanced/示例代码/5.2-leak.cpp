// 5.2 例题：自己计数，看「申请了不释放」是怎么累积起来的。
//
// 没有安装检查工具，就用两个计数器自己统计：每次 new 记一次，每次 delete 记一次。
// 第一版申请一个释放一个，两个数对得上；第二版申请十个一个不释放，就留下十块未释放的内存。
// 这十块内存一直占着，直到程序退出才被系统整体收回——
// 程序短，看不出问题；换成一个运行一整天的程序，这十块的数量会持续累积。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.2-leak.cpp -o /tmp/t && /tmp/t

#include <iostream>

int created = 0;        // new 了几次
int released = 0;       // delete 了几次

int* make_value(int x) {
    created = created + 1;
    int* p = new int;
    *p = x;
    return p;
}

void release_value(int* p) {
    released = released + 1;
    delete p;
}

int main() {
    std::cout << "—— 第一版：申请一个，释放一个 ——" << std::endl;
    int* a = make_value(1);
    std::cout << "取得 " << *a << std::endl;
    release_value(a);

    std::cout << "—— 第二版：申请十个，一个不释放 ——" << std::endl;
    for (int i = 0; i < 10; i++) {
        int* p = make_value(i);
        std::cout << "取得 " << *p << std::endl;
    }

    std::cout << "申请了 " << created << " 次，释放了 " << released
              << " 次。" << std::endl;
    std::cout << "未释放的内存有 " << created - released << " 块。" << std::endl;

    return 0;
}
