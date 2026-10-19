// 3.1：往里塞的对面——拿走一个、全倒掉、问它空不空。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.1-pop-clear.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {90, 85, 77};

    v.pop_back();
    std::cout << v.size() << std::endl;
    std::cout << v[1] << std::endl;

    v.clear();
    std::cout << v.size() << std::endl;

    if (v.empty()) {
        std::cout << "空了" << std::endl;
    }

    return 0;
}
