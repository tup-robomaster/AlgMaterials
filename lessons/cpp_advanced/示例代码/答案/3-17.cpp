// 练习 3-17 的答案。两个问题两个容器：
// 「一共几个」需要全部保留，用 vector；「几种」需要去重，用 set。

#include <iostream>
#include <set>
#include <string>
#include <vector>

int main() {
    int n;
    std::cin >> n;

    std::vector<std::string> all;
    std::set<std::string> kinds;

    for (int i = 0; i < n; i++) {
        std::string w;
        std::cin >> w;

        all.push_back(w);
        kinds.insert(w);
    }

    std::cout << "一共 " << all.size() << " 个" << std::endl;
    std::cout << "有 " << kinds.size() << " 种" << std::endl;

    std::cout << "它们是：";
    for (const std::string& w : kinds) {
        std::cout << w << " ";
    }
    std::cout << std::endl;

    return 0;
}
