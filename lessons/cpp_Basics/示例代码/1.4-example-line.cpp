// 1.4 例题：把几个值拼成一句话
//
// 一句话里有三段：文字、整数、小数。它们被三对 << 串在一起，
// 一次打完，最后换行。

#include <iostream>
#include <string>

int main() {
    std::string name = "小航";
    int age = 19;
    double height = 1.75;

    std::cout << name << "今年" << age << "岁，身高" << height << "米" << std::endl;

    return 0;
}
