// 4.3：erase 要的是位置，不是编号——这一份是故意编不过的。
//
// 从 3.1 起，想删东西的时候最容易一不留神写成 v.erase(2)，
// 意思是「删第 2 个」。编译器不认：它要的是个迭代器，
// 而 2 只是个 int，两样东西对不上号。
//
// 看报错：g++ -std=c++17 -Wall 示例代码/4.3-erase-index-err.cpp -o /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {10, 20, 30};

    v.erase(2);

    std::cout << v.size() << std::endl;

    return 0;
}
