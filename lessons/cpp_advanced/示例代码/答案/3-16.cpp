// 练习 3-16 的答案。queue 本身就是用于这个目的：
// push 添加元素，front 读取队头，pop 删除。

#include <iostream>
#include <queue>
#include <string>

int main() {
    int n;
    std::cin >> n;

    std::queue<std::string> q;

    for (int i = 0; i < n; i++) {
        std::string name;
        std::cin >> name;

        q.push(name);
    }

    int k = 1;

    while (!q.empty()) {
        std::cout << k << ". " << q.front() << std::endl;
        k++;
        q.pop();
    }

    return 0;
}
