// 4.6：自己定义的 struct，sort 直接排是无法编译的。
//
// 这一份就是列出来供人观察的：sort 只有一条规矩「a < b」，
// 而 Student 是我们自己定义的，谁也没有告诉过它两个 Student 怎么比较大小。
// 报错篇幅很长，其实只有一句话——找不到能用的 <。
//
// 改法在下一份：给它一条 lambda，说清楚按什么排。
//
// 看报错：g++ -std=c++17 -Wall 示例代码/4.6-sort-struct-err.cpp -o /tmp/t

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int score;
};

int main() {
    std::vector<Student> roster = {{"小李", 90}, {"小王", 77}};

    std::sort(roster.begin(), roster.end());

    std::cout << roster[0].name << std::endl;

    return 0;
}
