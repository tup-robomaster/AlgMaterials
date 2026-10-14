// 6.4 大东西别抄：同一个 string，值传递抄了一份，引用传递没抄
//
// 地址一比就清楚了（地址每次跑都不一样，但「哪两个一样」是稳定的）：
//   main 里 name 的地址，和 byRef 里 s 的地址，**一模一样**——是同一个盒子。
//   byValue 里 s 的地址不一样——它是一个新盒子，内容抄过来的。
//
// 抄一份 string 不只是多占地方：它得把每一个字符都搬一遍。
// 递进去的要是几万个数的 vector，这一下就白搬了好几万个。

#include <iostream>
#include <string>

void byValue(std::string s) {
    std::cout << "值传递   函数里的 s 住在 " << &s << std::endl;
    std::cout << "         内容是 " << s << std::endl;
}

void byRef(const std::string& s) {
    std::cout << "引用传递 函数里的 s 住在 " << &s << std::endl;
    std::cout << "         内容是 " << s << std::endl;
}

int main() {
    std::string name = "小明";

    std::cout << "main      name 自己住在 " << &name << std::endl;

    byValue(name);
    byRef(name);

    return 0;
}
