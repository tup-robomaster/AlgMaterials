// 6.5 那串字末尾藏着一个结尾记号
//
// "abc" 只有三个字符，可 sizeof("abc") 是 4——
// 末尾还藏着一个 '\0'（读作「空字符」），专门用来标「到这儿就完了」。
// cout 打一个 char* 能知道什么时候停，靠的就是它。
//
// 注意区分：sizeof(s) 量的不是那串字，是 s 这个**指针盒子**（8 格）。

#include <iostream>

int main() {
    const char* s = "abc";

    std::cout << "那串字占几格  " << sizeof("abc") << std::endl;
    std::cout << "s 这个盒子占几格  " << sizeof(s) << std::endl;

    return 0;
}
