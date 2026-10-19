// 练习 4-6 的答案：在 map 里找一个名字，找不到就说没有。
//
// find 找到的是「那个键值对的位置」，不是年龄；年龄在那个位置的
// second 里（键是 first），所以要 it->second。
// 找不到的时候它给的是 end()——那个「最后一格的后面」在这儿当记号用，
// 正好是「名单里没有这个人」。

#include <iostream>
#include <map>
#include <string>

int main() {
    std::map<std::string, int> age;
    age["小李"] = 19;
    age["小王"] = 20;
    age["小张"] = 18;

    std::string name;
    std::cin >> name;

    auto it = age.find(name);
    if (it == age.end()) {
        std::cout << "名单里没有 " << name << std::endl;
    } else {
        std::cout << it->first << "的年龄是 " << it->second << std::endl;
    }

    return 0;
}
