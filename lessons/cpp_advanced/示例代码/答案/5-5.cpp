// 练习 5-5 的答案：把自己管理的那块内存写成一个小类。
//
// 申请写在构造函数里、释放写在析构函数里，这一对就固定在同一个地方了：
// 使用这个类的人一句 delete 都不用写，也不应该写。
// 复制那条路关闭（= delete）——两份指针指向同一块内存，最后会释放两次。

#include <iostream>

class IntArray {
public:
    IntArray(int n) : data(new int[n]), count(n) {
        for (int i = 0; i < n; i++) {
            data[i] = 0;
        }
    }

    ~IntArray() {
        delete[] data;
    }

    IntArray(const IntArray&) = delete;

    void set(int i, int v) {
        data[i] = v;
    }

    int at(int i) const {
        return data[i];
    }

    int size() const {
        return count;
    }

    int sum() const {
        int s = 0;
        for (int i = 0; i < count; i++) {
            s = s + data[i];
        }
        return s;
    }

private:
    int* data;
    int count;
};

int main() {
    IntArray a(4);
    a.set(0, 10);
    a.set(1, 20);
    a.set(2, 30);
    a.set(3, 40);

    std::cout << "一共 " << a.size() << " 个元素" << std::endl;
    std::cout << "第 3 个元素是 " << a.at(2) << std::endl;
    std::cout << "加起来是 " << a.sum() << std::endl;

    return 0;
}
