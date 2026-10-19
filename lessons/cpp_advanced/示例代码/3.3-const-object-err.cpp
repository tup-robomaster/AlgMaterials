// 3.3：一个 const 的对象，只能叫得动带 const 的方法。
//
// 报错里那个 this 是基础课 5.3 讲过的「谁在调用我」。
// 编译器说的是：你递给我的这个 this 是只读的，可这个方法没答应过只读。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.3-const-object-err.cpp -o /tmp/t

#include <iostream>

class Battery {
public:
    int percent;

    Battery(int p) {
        percent = p;
    }

    void print() const {
        std::cout << percent << "%" << std::endl;
    }

    void drain() {
        percent = percent - 10;
    }
};

int main() {
    const Battery b(80);

    b.print();
    b.drain();

    return 0;
}
