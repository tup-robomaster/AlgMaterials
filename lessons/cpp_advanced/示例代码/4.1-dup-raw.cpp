// 4.1 例题：名单里有没有重名——最土的那个办法，两两碰一次面。
//
// 每两个名字都比一遍，比到了就记下来。人少的时候没感觉，
// 把「比了几次」打出来就不一样了：n 个名字要比 n×(n-1)/2 次。
// 输入里第 1 个和第 4 个是同一个名字。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.1-dup-raw.cpp -o /tmp/t && /tmp/t < 示例代码/4.1-dup.in

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

    int count = 0;
    bool found = false;
    for (int i = 0; i < int(name.size()); i++) {
        for (int j = i + 1; j < int(name.size()); j++) {
            count = count + 1;
            if (name[i] == name[j]) {
                found = true;
            }
        }
    }

    std::cout << n << " 个名字，两两比了 " << count << " 次" << std::endl;
    if (found) {
        std::cout << "有重名" << std::endl;
    } else {
        std::cout << "没有重名" << std::endl;
    }

    return 0;
}
