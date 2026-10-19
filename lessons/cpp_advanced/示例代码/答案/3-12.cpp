// 练习 3-12 的答案。times[w]++ 一句干了两件事：
// 没有 w 就先建一个（值默认 0），再给它加一。

#include <iostream>
#include <map>
#include <string>

int main() {
    int n;
    std::cin >> n;

    std::map<std::string, int> times;

    for (int i = 0; i < n; i++) {
        std::string w;
        std::cin >> w;

        times[w]++;
    }

    for (const auto& kv : times) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }

    return 0;
}
