// 5.4 例题：把「谁销毁」写进类型——unique_ptr。
//
// Counter 的析构函数里有一行打印，所以对象什么时候销毁，屏幕上看得到。
// 两段各执行一遍：第一段让它离开花括号时自动销毁；第二段移动一次，
// 两个名字先后指向同一个对象，最后也还是只销毁一次。
//
// 认准一件事：整份程序里没有一个 delete。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.4-unique.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <memory>

class Counter {
public:
    Counter(int start) : value(start) {
        std::cout << "（创建一个对象，初始值 " << value << "）" << std::endl;
    }

    ~Counter() {
        std::cout << "（销毁——不需要手动处理）" << std::endl;
    }

    void add(int n) {
        value = value + n;
    }

    int get() const {
        return value;
    }

private:
    int value;
};

int main() {
    std::cout << "—— 第一段：出了花括号就销毁 ——" << std::endl;
    {
        std::unique_ptr<Counter> c = std::make_unique<Counter>(10);
        c->add(5);
        c->add(3);
        std::cout << "现在是 " << c->get() << std::endl;
    }
    std::cout << "（离开花括号）" << std::endl;

    std::cout << "—— 第二段：移动一次 ——" << std::endl;
    {
        std::unique_ptr<Counter> a = std::make_unique<Counter>(100);
        std::unique_ptr<Counter> b = std::move(a);
        b->add(1);
        std::cout << "现在轮到 b 用它：" << b->get() << std::endl;
    }
    std::cout << "（离开花括号——两个名字，只销毁一次）" << std::endl;

    return 0;
}
