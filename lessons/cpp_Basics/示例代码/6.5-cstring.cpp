// 6.5 字符串：一个地址，后面跟着一串字符
//
// s 里装的是一个地址——那串字的头一个字符在哪儿。
// cout 打 s 会一路打到「结尾记号」为止，所以能打出整个词；
// 打 s[1] 就只打第二个字符。
//
// const 的意思是「这些字只许读，不许改」。

#include <iostream>

int main() {
    const char* s = "abc";

    std::cout << s << std::endl;         // 从 s 指的地方一路打下去
    std::cout << s[1] << std::endl;      // 第二个字符
    std::cout << *(s + 2) << std::endl;  // 跟 s[2] 是一回事

    return 0;
}
