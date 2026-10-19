// 3.1：大括号里直接初始化。
//
// 基础课 4.1 那个 too many initializers——内存足够，
// 但元素个数固定在方括号里，第六个数无法放入。
// 换成 vector，元素个数不再固定，放几个都可以。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.1-init-list.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<int> v = {90, 85, 77, 60, 95, 88};   // 六个

    std::cout << v.size() << std::endl;
    std::cout << v[5] << std::endl;                  // 第六个，下标是 5

    std::vector<std::string> names = {"小明", "小红", "小刚"};
    std::cout << names[1] << std::endl;

    std::vector<int> zeros(3);                       // 圆括号：要 3 个空位
    std::cout << zeros.size() << " " << zeros[0] << std::endl;

    return 0;
}
