// 练习 3-5 的答案。读一整行用 getline，倒序输出依靠下标。
// 输入里全是 ASCII，一个字节一个字符，倒序输出正好就是倒序读取。

#include <iostream>
#include <string>

int main() {
    std::string line;
    std::getline(std::cin, line);

    for (int i = int(line.size()) - 1; i >= 0; i--) {
        std::cout << line[i];
    }
    std::cout << std::endl;

    return 0;
}
