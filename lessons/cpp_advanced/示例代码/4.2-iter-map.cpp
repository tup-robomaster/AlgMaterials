// 4.2：迭代器不挑容器——vector 有，string 有，map 也有。
//
// 不一样的地方在「*it 拿出来的是什么」：
//   vector<int> 拿出来的是 int；string 拿出来的是一个字符；
//   map 取得的是一个 pair（3.4 学的那种组合类型），
//   所以访问其中的成员要用 it->first、it->second。
//   （写成 (*it).first 一模一样，只是多打几个字。）
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.2-iter-map.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> age;
    age["小李"] = 19;
    age["小王"] = 20;
    age["小张"] = 18;

    for (auto it = age.begin(); it != age.end(); ++it) {
        std::cout << it->first << " " << it->second << std::endl;
    }

    std::cout << "--- 逐个字符遍历 ---" << std::endl;

    std::string s = "abc";
    for (auto it = s.begin(); it != s.end(); ++it) {
        std::cout << *it << std::endl;
    }

    return 0;
}
