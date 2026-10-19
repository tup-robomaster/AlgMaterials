// 3.2：读一整行，顺便找出最长的那一行。
//
// 跟 3.2-getline-trap.cpp 只差一行：读完 n 之后把那一行剩下的东西倒掉。
// 两份喂的是同一份输入，并排跑一遍，差别一眼就看见了。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.2-getline.cpp -o /tmp/t &&
//         /tmp/t < 示例代码/3.2-getline.in

#include <iostream>
#include <string>

int main() {
    int n;
    std::cin >> n;
    std::cin.ignore();          // 把 n 后面那个回车丢掉

    std::string longest;

    for (int i = 0; i < n; i++) {
        std::string line;
        std::getline(std::cin, line);

        std::cout << i + 1 << ": " << line << std::endl;

        if (line.size() > longest.size()) {
            longest = line;
        }
    }

    std::cout << "最长的一行是 " << longest << std::endl;

    return 0;
}
