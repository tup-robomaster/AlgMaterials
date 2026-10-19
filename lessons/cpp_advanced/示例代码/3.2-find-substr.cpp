// 3.2：在一串字里找一段——find 告诉你从第几个字节起，substr 把那一段剪下来。
//
// 这是从一行文本里抠数据的标准姿势：先找到标签，再从标签后面剪。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.2-find-substr.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>

int main() {
    std::string line = "id=42, score=90";

    int p = int(line.find("score="));
    std::cout << p << std::endl;

    std::string value = line.substr(p + 6);
    std::cout << value << std::endl;

    std::cout << line.substr(0, 2) << std::endl;   // 从头起截取两个字节

    if (line.find("id=") != std::string::npos) {
        std::cout << "有 id" << std::endl;
    }
    if (line.find("name=") == std::string::npos) {
        std::cout << "没有 name" << std::endl;
    }

    return 0;
}
