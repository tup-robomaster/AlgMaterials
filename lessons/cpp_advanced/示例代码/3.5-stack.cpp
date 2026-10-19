// 3.5：stack——栈，后进先出。最后添加的元素最先被删除。
//
// 名字跟 queue 只差一个词，规则正好相反：queue 只能访问队头，stack 只能访问栈顶。
//
// 运行一下：g++ -std=c++17 -Wall 示例代码/3.5-stack.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <stack>
#include <string>

int main() {
    std::stack<std::string> back;      // 用作访问历史：后访问的先返回

    back.push("主页");
    back.push("课程");
    back.push("第 3 部分");

    std::cout << back.top() << std::endl;    // 栈顶元素

    back.pop();
    std::cout << back.top() << std::endl;

    std::cout << back.size() << std::endl;

    return 0;
}
