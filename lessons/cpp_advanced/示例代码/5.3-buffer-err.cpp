// 5.3 错例（故意编不过）：从一个自己管理内存的对象复制出另一份。
//
// 「从 a 复制出 b」走的是拷贝构造那条路，5.3-buffer.cpp 里
// 把它标成了 = delete。复制一份为什么不行：两个对象指向同一块内存，
// 各自析构的时候会把同一块内存释放两次。
// 编译器给出的报错不长，页面上的报错就是这一份实际报出来的。
//
// 编译一下（看报错）：g++ -std=c++17 -Wall 示例代码/5.3-buffer-err.cpp -o /tmp/t

#include <iostream>

class Buffer {
public:
    Buffer(int n) : data(new int[n]), count(n) {}
    ~Buffer() { delete[] data; }

    Buffer(const Buffer&) = delete;     // 这一条禁止了复制

    int size() const { return count; }

private:
    int* data;
    int count;
};

int main() {
    Buffer a(5);
    Buffer b = a;       // 从 a 复制一个——被阻止在这一行
    std::cout << b.size() << std::endl;
    return 0;
}
