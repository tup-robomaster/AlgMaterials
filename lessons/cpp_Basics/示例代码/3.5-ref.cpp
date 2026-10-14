// 3.5：加一个 &，递进去的就不是复印件了。
//
// 和 3.4-copy.cpp 只差一个字符：括号里 int 后面多了个 &。
// 加上它，x 就不再是新盒子，而是 a 的**另一个名字**——
// 往 x 里写，就是往 a 里写。
//
// 跑一下：g++ -std=c++17 -Wall 示例代码/3.5-ref.cpp -o /tmp/t && /tmp/t

#include <iostream>

void addOne(int& x) {
    x = x + 1;
    std::cout << "函数里 x = " << x << std::endl;
}

int main() {
    int a = 3;
    addOne(a);
    std::cout << "回来 a = " << a << std::endl;
    return 0;
}
