// 4.3：删中间那个——3.1 欠的账，这一课还。
//
// erase 要的是「哪个位置」，不是「第几个」：位置拿迭代器说，
// v.begin() + 2 读作「头一格再往后跨两格」，也就是第 2 个（从 0 数）。
// 把编号一起打出来，是为了看清删完之后的挪动：
// 原来站在第 3 个的 40，删完之后站在第 2 个上——它自己往前挪了一格。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.3-erase.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {10, 20, 30, 40, 50};

    std::cout << "删之前：" << std::endl;
    for (int i = 0; i < int(v.size()); i++) {
        std::cout << "第 " << i << " 个：" << v[i] << std::endl;
    }

    v.erase(v.begin() + 2);

    std::cout << "删之后：" << std::endl;
    for (int i = 0; i < int(v.size()); i++) {
        std::cout << "第 " << i << " 个：" << v[i] << std::endl;
    }

    return 0;
}
