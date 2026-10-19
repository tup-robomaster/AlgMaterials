// 4.2：上一份那四步，写成 for 就是一行。
//
// 「从头走到尾」这件事，走的永远是同一个套路：
// 起点 begin()、终点 end()、每次循环 ++、循环体中使用 *it。
// 3.1 学的范围 for 就是把这一行藏起来了——底下干的是同一件事。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.2-iter-for.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<int> score = {90, 85, 77};

    for (auto it = score.begin(); it != score.end(); ++it) {
        std::cout << *it << std::endl;
    }

    return 0;
}
