// 练习 5-6 的答案：让构造函数和析构函数各打印一行。
//
// 这样就能直接观察到两件事：
//   一、对象在哪个花括号里创建，就在哪个花括号结束时销毁；
//   二、同一个花括号里的几个对象，销毁顺序与构造顺序相反——
//       后构造的先销毁，最后构造的那个最先销毁。

#include <iostream>
#include <string>

class Marker {
public:
    Marker(std::string name) : name(name) {
        std::cout << "构造：" << name << std::endl;
    }

    ~Marker() {
        std::cout << "析构：" << name << std::endl;
    }

    std::string name;
};

int main() {
    std::cout << "—— 进外层 ——" << std::endl;
    {
        Marker a("甲");
        Marker b("乙");

        std::cout << "—— 进内层 ——" << std::endl;
        {
            Marker c("丙");
        }
        std::cout << "—— 出内层 ——" << std::endl;
    }
    std::cout << "—— 出外层 ——" << std::endl;

    return 0;
}
