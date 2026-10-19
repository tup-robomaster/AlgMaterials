// 6.1：反面例子——用 while (!in.eof()) 判断读完了没有。
// 最后一次读取失败时，变量里还是上一次的内容，于是最后一行会被输出两次。
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream in("6.1-names.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }

    std::string name;
    while (!in.eof()) {
        in >> name;
        std::cout << "读到：" << name << std::endl;
    }

    return 0;
}
