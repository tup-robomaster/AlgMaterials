// 5.3 例题：把「释放」写进析构函数——出口有几条都不会遗漏。
//
// 和 5.3-manual.cpp 是同一件事：一样的两个出口，一样的两趟调用。
// 区别只有一处：这一版没有哪条出口上写着 delete[]，
// 因为「释放」不在出口上，而在析构函数里——对象什么时候销毁，它就什么时候释放。
// 构造函数和析构函数的打印一一对应，运行结果一眼就能看见：
// 哪怕函数是从中途返回的，内存也照样释放了。
//
// 复制那条路关闭了（= delete）：两份 Buffer 指向同一块内存的话，
// 两个析构函数会把同一块内存释放两次——就是 5.2 那个崩溃。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.3-buffer.cpp -o /tmp/t && /tmp/t

#include <iostream>

class Buffer {
public:
    Buffer(int n) : data(new int[n]), count(n) {
        std::cout << "（申请一块能存放 " << count << " 个元素的内存）" << std::endl;
    }

    ~Buffer() {
        delete[] data;
        std::cout << "（释放完成）" << std::endl;
    }

    Buffer(const Buffer&) = delete;     // 禁止复制，避免两个对象指向同一处

    void set(int i, int v) {
        data[i] = v;
    }

    int get(int i) const {
        return data[i];
    }

    int size() const {
        return count;
    }

private:
    int* data;
    int count;
};

int count_until_over(int n, int limit) {
    Buffer buf(n);
    for (int i = 0; i < n; i++) {
        buf.set(i, i + 1);
    }

    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum = sum + buf.get(i);
        if (sum > limit) {
            std::cout << "（中途返回的出口——这里没有写任何释放语句）" << std::endl;
            return i + 1;
        }
    }

    std::cout << "（正常结束的出口——这里也没有写任何释放语句）" << std::endl;
    return n;
}

int main() {
    std::cout << "限 10：" << count_until_over(10, 10) << " 个数" << std::endl;
    std::cout << "限 100：" << count_until_over(10, 100) << " 个数" << std::endl;
    return 0;
}
