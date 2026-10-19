// 6.1：一次读一整行，行里有空格也照样读进来。
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream in("6.1-lines.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }

    std::string line;
    int no = 1;
    while (std::getline(in, line)) {
        std::cout << no << ": " << line << std::endl;
        no++;
    }

    return 0;
}
