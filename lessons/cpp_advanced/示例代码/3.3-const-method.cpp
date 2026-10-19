// 3.3：const 写在自己定义的类上——「这个方法不碰对象里的东西」。
//
// 位置在参数表那个括号后面、大括号前面。基础课 5.2 说成员函数就是把函数
// 搬进大括号里，这个 const 是搬进来之后才有的一个位置。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.3-const-method.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>

class Battery {
public:
    int percent;

    Battery(int p) {
        percent = p;
    }

    void print() const {            // 只看，不动
        std::cout << percent << "%" << std::endl;
    }

    void drain() {                  // 这个要动，所以不写 const
        percent = percent - 10;
    }
};

int main() {
    Battery b(80);

    b.print();
    b.drain();
    b.print();

    return 0;
}
