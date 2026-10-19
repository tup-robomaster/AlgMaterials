// 练习 4-13 的答案：排完再比挨着的两个。
//
// 4.1 那两版都是「两两比较」，这一版把排序当工具用：
// 排完之后，一样的两个名字必然挨在一起，所以只要走一趟，
// 每次比 v[i] 和 v[i+1] 就够了。
// 代价是排序那一下 O(n log n)——比两两比较的 O(n²) 便宜得多，
// 比「问 set」的 O(n) 贵一点，但省掉了 set 那份内存。

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    int n = 0;
    std::cin >> n;

    std::vector<std::string> name;
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;
        name.push_back(s);
    }

    std::sort(name.begin(), name.end());

    std::cout << "排完：";
    for (const std::string& s : name) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    bool dup = false;
    for (int i = 0; i + 1 < int(name.size()); i++) {
        if (name[i] == name[i + 1]) {
            dup = true;
        }
    }

    std::cout << (dup ? "有重名" : "没有重名") << std::endl;

    return 0;
}
