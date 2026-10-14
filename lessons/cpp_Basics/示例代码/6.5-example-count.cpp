// 6.5 例题：自己数一数字符串有多长。
//
// 那串字末尾藏着 '\0'，从 s 指的格子一路数过去，碰上它就停。
// 最后一行拿 sizeof 比一比：sizeof 量的是 s 这个盒子，不是那串字本身。

#include <iostream>

int length(const char* s) {
    int n = 0;

    while (s[n] != '\0') {
        n = n + 1;
    }

    return n;
}

int main() {
    const char* a = "hi";
    const char* b = "HelloWorld";

    std::cout << a << " 有 " << length(a) << " 个字符" << std::endl;
    std::cout << b << " 有 " << length(b) << " 个字符" << std::endl;
    std::cout << "sizeof(b) 量出来 " << sizeof(b) << std::endl;

    return 0;
}
