// 6.3：异常向外传递时，沿途的局部对象会依次销毁。
#include <iostream>
#include <stdexcept>
#include <string>

class Guard {
public:
    Guard(const std::string& name) : name(name) {
        std::cout << "创建 " << name << std::endl;
    }
    ~Guard() {
        std::cout << "销毁 " << name << std::endl;
    }

private:
    std::string name;
};

void inner() {
    Guard g1("内层对象");
    throw std::runtime_error("内层出错");
}

void outer() {
    Guard g2("外层对象");
    inner();
    std::cout << "这一行不会执行" << std::endl;
}

int main() {
    try {
        outer();
    } catch (const std::exception& e) {
        std::cout << "捕获：" << e.what() << std::endl;
    }

    return 0;
}
