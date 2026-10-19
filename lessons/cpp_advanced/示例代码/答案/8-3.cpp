#include <iostream>
#include <utility>

class Buffer {
public:
    explicit Buffer(int n) : size_(n), data_(new int[n]) {
        for (int i = 0; i < size_; i++) {
            data_[i] = 0;
        }
    }

    ~Buffer() {
        delete[] data_;
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other)
        : size_(other.size_), data_(other.data_) {
        other.size_ = 0;
        other.data_ = nullptr;
    }

    int size() const {
        return size_;
    }

    bool empty() const {
        return data_ == nullptr;
    }

    int& at(int i) {
        return data_[i];
    }

    const int& at(int i) const {
        return data_[i];
    }

private:
    int size_;
    int* data_;
};

int main() {
    Buffer a(4);
    for (int i = 0; i < a.size(); i++) {
        a.at(i) = (i + 1) * 10;
    }

    std::cout << "移动前 a：" << a.size() << "\n";

    Buffer b(std::move(a));

    std::cout << "移动后 a：" << a.size() << "\n";
    std::cout << "移动后 a 为空：" << (a.empty() ? "是" : "否") << "\n";
    std::cout << "移动后 b：" << b.size() << "\n";

    int sum = 0;
    for (int i = 0; i < b.size(); i++) {
        sum += b.at(i);
    }
    std::cout << "b 的和：" << sum << "\n";
    return 0;
}
