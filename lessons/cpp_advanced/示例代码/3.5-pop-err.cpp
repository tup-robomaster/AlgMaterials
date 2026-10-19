// 3.5：pop 不给你东西。
//
// 这一条最容易记反：vector 的 pop_back() 也不给，但那个是
// 「拿走最后那个」，脑子里总以为它会交回来。queue 和 stack 一样，
// pop 只管拿掉——想要那个东西，先用 front()（或 top()）看一眼。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.5-pop-err.cpp -o /tmp/t

#include <iostream>
#include <queue>

int main() {
    std::queue<int> q;
    q.push(1);

    int x = q.pop();

    std::cout << x << std::endl;

    return 0;
}
