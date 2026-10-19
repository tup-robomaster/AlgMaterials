// 4.5：给 sort 传一条 lambda，它就能从大到小排序。
//
// sort 的第三个参数是「谁该排在前面」：给它一段代码，
// 回答「a 该排在 b 前面吗」。默认的那条是 a < b，所以从小到大；
// 换成 a > b，大的就排在前面。
//
// 这一份是下一课的引子——连 int 都能换规矩，
// 自己定义的 struct 当然也能，而且非给不可。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.5-sort-lambda.cpp -o /tmp/t && /tmp/t

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> score = {45, 52, 68, 91, 73};

    std::sort(score.begin(), score.end(), [](int a, int b) { return a > b; });

    std::cout << "从高到低：";
    for (int x : score) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
