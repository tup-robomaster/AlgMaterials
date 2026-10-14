// 练习 3-3 的答案：三个三角形（2 行、4 行、6 行）按老办法塞进一个 main。
//
// 三段的差别只有那个上限数字，可每一段都得整段抄一遍——
// 一共 40 多行，其中 30 行是重复的。
// 这就是 3.2 要解决的事：把它变成十几行。

#include <iostream>

int main() {
    int i = 1;
    int j = 1;

    // ── 2 行的 ──
    i = 1;
    while (i <= 2) {
        j = 1;
        while (j <= i) {
            std::cout << "*";
            j = j + 1;
        }
        std::cout << std::endl;
        i = i + 1;
    }

    std::cout << "----" << std::endl;

    // ── 4 行的 ──
    i = 1;
    while (i <= 4) {
        j = 1;
        while (j <= i) {
            std::cout << "*";
            j = j + 1;
        }
        std::cout << std::endl;
        i = i + 1;
    }

    std::cout << "----" << std::endl;

    // ── 6 行的 ──
    i = 1;
    while (i <= 6) {
        j = 1;
        while (j <= i) {
            std::cout << "*";
            j = j + 1;
        }
        std::cout << std::endl;
        i = i + 1;
    }

    return 0;
}
