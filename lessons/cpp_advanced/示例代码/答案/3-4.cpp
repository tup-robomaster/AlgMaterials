// 练习 3-4 的答案。读一张表，按列输出。
// 读的时候一行一行来：里面那个 vector 装当前这一行，读完 push_back 进外面那个。
// 输出时两重循环交换次序——外层遍历列、内层遍历行。

#include <iostream>
#include <vector>

int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<int>> grid;

    for (int i = 0; i < n; i++) {
        std::vector<int> row;
        for (int j = 0; j < m; j++) {
            int x;
            std::cin >> x;
            row.push_back(x);
        }
        grid.push_back(row);
    }

    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
