// 3.3：出了地盘还想用，编译器不让。
//
// 故意写错的，用来看报错长什么样。
// 大括号里面 b 用得好好的，一出来就不认识了。
//
// 别改这个文件——改了行号就对不上课里那段报错了。

#include <iostream>

int main() {
    {
        int b = 2;
        std::cout << "里面 b = " << b << std::endl;
    }

    std::cout << "外面 b = " << b << std::endl;
    return 0;
}
