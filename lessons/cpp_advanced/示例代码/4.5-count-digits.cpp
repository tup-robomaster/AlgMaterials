// 4.5 例题：一句话里数出几个数字、几个大写字母。
//
// 字符能直接比较大小：'0' 到 '9' 在字符表里是相邻的，
// 所以「>='0' 而且 <='9'」判断的就是「这是不是个数字」。
// 条件写成 lambda，统计交给 count_if——一个循环都没有写。
//
// 补充一点：一行里有中文也不影响，中文的那几个字节
// 既不落在数字那一段，也不落在大写字母那一段。
//
// 输入：一整行（可能带空格），用 getline 读，3.2 学的。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.5-count-digits.cpp -o /tmp/t && /tmp/t < 示例代码/4.5-count-digits.in

#include <algorithm>
#include <iostream>
#include <string>

int main() {
    std::string line;
    std::getline(std::cin, line);

    int digits = std::count_if(line.begin(), line.end(),
                               [](char c) { return c >= '0' && c <= '9'; });

    int upper = std::count_if(line.begin(), line.end(),
                              [](char c) { return c >= 'A' && c <= 'Z'; });

    std::cout << line << std::endl;
    std::cout << "数字 " << digits << " 个，大写字母 " << upper << " 个" << std::endl;

    return 0;
}
