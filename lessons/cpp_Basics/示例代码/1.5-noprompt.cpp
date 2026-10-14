// 1.5 补充：没有提示语的样子
//
// 和例题做同一件事，只是把两句 "第一个数：" 删掉了。
// 程序本身没毛病——它安静地等你，光标在那儿闪，就看你知道不知道要敲。

#include <iostream>

int main() {
    int a;
    int b;

    std::cin >> a;
    std::cin >> b;

    std::cout << a + b << std::endl;

    return 0;
}
