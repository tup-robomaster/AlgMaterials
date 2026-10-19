// 4.2：不是所有迭代器都能 +1。
//
// vector 的能——它的元素连续存放，说「往后第 3 个」它一次就能定位。
// map 的不能：它的底层是树，元素之间没有「相邻」关系，只支持 ++ 逐个移动。
// 这一份是故意编不过的，课里贴的就是它报出来的那句。
//
// 看报错：g++ -std=c++17 -Wall 示例代码/4.2-iter-plus-err.cpp -o /tmp/t

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> age;
    age["小李"] = 19;
    age["小王"] = 20;

    auto it = age.begin();
    it = it + 1;

    std::cout << it->first << std::endl;

    return 0;
}
