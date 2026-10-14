// 1.5 补充：读一段文字，它在哪儿停
//
// 给 std::string 读的时候，> 读到空白就停——空格、回车都算空白。

#include <iostream>
#include <string>

int main() {
    std::string word;

    std::cin >> word;
    std::cout << "读到的是：" << word << std::endl;

    return 0;
}
