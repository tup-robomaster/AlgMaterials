// 4.4：sort——从小到大排，一句话。
//
// 排数字好懂。排字符串「哪个在前」是另一回事：
// 它比较的是逐个字符的编号，即字节，不是拼音。
// 三个「小」开头的名字，排出来是「小张、小李、小王」——
// 看起来没有规律，其实是第二个字在 UTF-8 里的字节大小。3.4 那个 map
// 按键输出时的顺序也是它，两边一致。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.4-sort.cpp -o /tmp/t && /tmp/t

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<int> v = {45, 52, 68, 91, 73};
    std::sort(v.begin(), v.end());
    std::cout << "排完：";
    for (int x : v) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    std::vector<std::string> words = {"banana", "apple", "cherry"};
    std::sort(words.begin(), words.end());
    std::cout << "英文排完：";
    for (const std::string& w : words) {
        std::cout << w << " ";
    }
    std::cout << std::endl;

    std::vector<std::string> names = {"小李", "小王", "小张"};
    std::sort(names.begin(), names.end());
    std::cout << "中文排完：";
    for (const std::string& w : names) {
        std::cout << w << " ";
    }
    std::cout << std::endl;

    return 0;
}
