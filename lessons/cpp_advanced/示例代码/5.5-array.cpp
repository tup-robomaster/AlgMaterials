// 5.5 例题：std::array——定长的数组，提供容器的那一套功能。
//
// 基础课 4.1 那个数组：元素个数固定在代码里，但 sizeof 一进函数就变小、
// 不能整体复制给其他对象、也不知道自己有几个。std::array 把这些补齐了，
// 「几个」仍然固定在类型里——尖括号里第二个数就是它。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.5-array.cpp -o /tmp/t && /tmp/t

#include <array>
#include <iostream>

int main() {
    std::array<int, 5> score = {90, 85, 77, 60, 95};

    std::cout << "有几个：" << score.size() << std::endl;

    int sum = 0;
    for (int x : score) {
        sum = sum + x;
    }
    std::cout << "总分：" << sum << std::endl;
    std::cout << "第一个：" << score[0] << "，最后一个：" << score[score.size() - 1]
              << std::endl;

    std::array<int, 5> copy = score;     // 整体复制给另一个对象——C 数组做不到这一条
    copy[0] = 0;
    std::cout << "复制一份、修改之后：原对象 " << score[0] << "，副本 " << copy[0]
              << std::endl;

    return 0;
}
