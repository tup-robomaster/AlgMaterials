// 练习 6-2：逐行读文件，统计行数和字符数。
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream in("6-2-in.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }

    std::string line;
    int lines = 0;
    int chars = 0;
    while (std::getline(in, line)) {
        lines++;
        chars = chars + int(line.size());
    }

    std::cout << lines << " 行，共 " << chars << " 个字符" << std::endl;
    return 0;
}
