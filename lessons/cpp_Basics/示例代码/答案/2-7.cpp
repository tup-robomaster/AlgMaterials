// 练习 2-7：读一个字符，判断它是小写字母、大写字母，还是别的。
//
// char 之间也能比大小——比的是字符在表里的次序。

#include <iostream>

int main() {
    char c = ' ';
    std::cin >> c;

    if (c >= 'a' && c <= 'z') {
        std::cout << "小写字母" << std::endl;
    } else if (c >= 'A' && c <= 'Z') {
        std::cout << "大写字母" << std::endl;
    } else {
        std::cout << "别的" << std::endl;
    }

    return 0;
}
