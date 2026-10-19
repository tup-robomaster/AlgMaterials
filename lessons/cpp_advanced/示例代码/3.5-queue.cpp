// 3.5：queue——队列，先进先出。
//
// 跟 vector 一样按顺序添加元素，差别在于从哪一端读取：vector 的任意下标都可以读取，
// queue 只允许读取队头——而且读取后该元素即被删除。
//
// 运行一下：g++ -std=c++17 -Wall 示例代码/3.5-queue.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <queue>
#include <string>

int main() {
    std::queue<std::string> line;

    line.push("小明");
    line.push("小红");
    line.push("小刚");

    std::cout << line.size() << std::endl;
    std::cout << line.front() << std::endl;    // 队头是谁

    while (!line.empty()) {
        std::cout << line.front() << " 出队" << std::endl;
        line.pop();                            // 队头出队
    }

    std::cout << line.size() << std::endl;

    return 0;
}
