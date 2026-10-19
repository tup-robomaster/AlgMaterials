// 3.3：答应过不改，就真改不了了。
//
// 跟基础课 6.4 那份 6.4-const-ref-err.cpp 是同一句报错，只是这回改的是 vector。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/3.3-modify-err.cpp -o /tmp/t

#include <iostream>
#include <vector>

void addOne(const std::vector<int>& v) {
    v.push_back(1);
}

int main() {
    std::vector<int> data = {90, 85, 77};

    addOne(data);

    return 0;
}
