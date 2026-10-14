// 0.2 一个 C++ 程序的骨架 —— 补充：main 里不写 return 0 也能编译
//
// C++ 规定 main 函数就算不写 return 0，程序正常结束时也自动返回 0。
// 所以下面这个程序没有问题。写不写都行，但建议写——别人看代码时
// 一眼就知道「这个程序正常结束」。

#include <iostream>

int main() {
    std::cout << "没有写 return 0，照样跑" << std::endl;
}
