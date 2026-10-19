// 4.5：界限变量放在外面——方括号里必须写上它的名字。
//
// lambda 里使用外部变量，规则只有一条：要使用谁，就在方括号里写上谁。
// 不写，它就无法访问外部（这是有意设计的：写明了才能知道这段代码依赖什么）。
// 写上之后，外面那个数改了，它跟着改——上一份用的是内外各一份的数，
// 这一份用的是外面这个。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.5-capture.cpp -o /tmp/t && /tmp/t

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<int> score = {45, 52, 68, 91, 73};

    int line = 60;
    auto it = std::find_if(score.begin(), score.end(),
                           [line](int x) { return x >= line; });
    std::cout << "及格线定为 " << line << "：第一个达到的是 " << *it << std::endl;

    line = 90;
    auto high = std::find_if(score.begin(), score.end(),
                             [line](int x) { return x >= line; });
    std::cout << "及格线定为 " << line << "：第一个达到的是 " << *high << std::endl;

    return 0;
}
