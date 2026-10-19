// 练习 3-7 的答案。两个 string 比大小、比长短，都跟 int 一样是
// 直接拿 < > == 去比，不用自己一个字节一个字节看。

#include <iostream>
#include <string>

int main() {
    std::string a;
    std::string b;
    std::cin >> a >> b;

    if (a == b) {
        std::cout << "两个词一样" << std::endl;
    } else {
        std::cout << "两个词不一样" << std::endl;
    }

    std::cout << a << " 有 " << a.size() << " 个字节" << std::endl;
    std::cout << b << " 有 " << b.size() << " 个字节" << std::endl;

    if (a.size() > b.size()) {
        std::cout << "长的是 " << a << std::endl;
    } else if (b.size() > a.size()) {
        std::cout << "长的是 " << b << std::endl;
    } else {
        std::cout << "一样长" << std::endl;
    }

    return 0;
}
