// 5.5 例题：std::move——移动，不复制第二份。
//
// 移动之后那个「源」还在，但里面已经空了。标准只保证它「还能安全地使用
// （销毁、赋新值都行）」，不一定保证是空的；这一份实现里结果就是 0。
// 所以移动之后不要再用原来的名字去读——要使用的是移动过去的那个新名字。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.5-move.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::vector<std::string> a;
    a.push_back("小李");
    a.push_back("小王");
    a.push_back("小张");

    std::cout << "移动之前：a 有 " << a.size() << " 个" << std::endl;

    std::vector<std::string> b = std::move(a);

    std::cout << "移动之后：b 有 " << b.size() << " 个，a 有 " << a.size()
              << " 个" << std::endl;
    std::cout << "b 里第一个是 " << b[0] << std::endl;

    std::cout << "—— 再移动一个字符串看看 ——" << std::endl;
    std::string s = "一段话";
    std::cout << "移动之前：s 是「" << s << "」，长度 " << s.size() << std::endl;

    std::string t = std::move(s);

    std::cout << "移动之后：t 是「" << t << "」，长度 " << t.size()
              << "；s 的长度 " << s.size() << std::endl;

    return 0;
}
