// 练习 3-19 的答案（综合练习）。三个容器各领一件事：
//   vector —— 只问「几行」，那就全留着，一个不落
//   set    —— 问「有哪几个人」，去重自带排序
//   map    —— 问「每个人几分」，同一个名字来几次取最高的那次

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

int main() {
    int n;
    std::cin >> n;

    std::vector<std::string> all;
    std::set<std::string> who;
    std::map<std::string, int> best;

    for (int i = 0; i < n; i++) {
        std::string name;
        int score;
        std::cin >> name >> score;

        all.push_back(name);
        who.insert(name);

        if (best.count(name) == 0) {
            best[name] = score;
        } else if (score > best[name]) {
            best[name] = score;
        }
    }

    std::cout << "一共 " << all.size() << " 人次" << std::endl;

    std::cout << "有 " << who.size() << " 个人：";
    for (const std::string& s : who) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    std::cout << "成绩单：" << std::endl;
    for (const auto& kv : best) {
        std::cout << "  " << kv.first << " " << kv.second << std::endl;
    }

    return 0;
}
