// 4.3：往中间插一个。
//
// insert 的括号里也是「位置，东西」：位置说插在谁前面，东西是插进去的值。
// 插进去之后，那个位置原来的东西往后挪一格。插在中间和插在最前面，
// 挪动的量差得远——插在最前面，整排都要往后挪，这就是它贵的原因。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.3-insert.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {10, 20, 40};

    v.insert(v.begin() + 2, 30);

    std::cout << "插在中间之后：" << std::endl;
    for (int i = 0; i < int(v.size()); i++) {
        std::cout << "第 " << i << " 个：" << v[i] << std::endl;
    }

    v.insert(v.begin(), 0);

    std::cout << "又插在最前面：" << std::endl;
    for (int i = 0; i < int(v.size()); i++) {
        std::cout << "第 " << i << " 个：" << v[i] << std::endl;
    }

    return 0;
}
