// 4.3：一边走一边删——循环里删东西的那个标准写法。
//
// 关键是 it = score.erase(it) 里那个等号：erase 会把它后面那些
// 往前挪一个位置，挪完原来的位置就失效了，所以它把「现在该看哪儿」还给你，
// 你接住，下一圈从那儿接着走。没删的时候才 ++it。
//
// 注意 for 的第三格是空的——往后走这件事，两个分支里各说各的。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.3-erase-loop.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

int main() {
    std::vector<int> score = {45, 90, 52, 68, 30, 91};

    for (auto it = score.begin(); it != score.end(); ) {
        if (*it < 60) {
            it = score.erase(it);
        } else {
            ++it;
        }
    }

    std::cout << "及格的：";
    for (int x : score) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
