// 4.1 例题：同一个问题，换成 set（3.5 学的那个）。
//
// 每读一个名字先问一句「这里面有没有」，没有就塞进去；有，就是重名。
// 一个名字问一次，问的次数是 n——不跟别人两两比较。
// 每次问要花几步，4.1 末尾那个「砍一半」讲了：set 里面是有序的，
// 一次问花的是 log n 步上下。合起来 n 个 log n，
// 跟 n² 摆在一起就是两个世界。
//
// 跟 4.1-dup-raw.cpp 吃同一份输入，两个程序的「比了几次」对着看。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.1-dup-set.cpp -o /tmp/t && /tmp/t < 示例代码/4.1-dup.in

#include <iostream>
#include <set>
#include <string>

int main() {
    int n = 0;
    std::cin >> n;

    std::set<std::string> seen;
    int ask = 0;
    bool found = false;
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;

        ask = ask + 1;
        if (seen.count(s) > 0) {
            found = true;
        }
        seen.insert(s);
    }

    std::cout << n << " 个名字，问了 " << ask << " 次" << std::endl;
    if (found) {
        std::cout << "有重名" << std::endl;
    } else {
        std::cout << "没有重名" << std::endl;
    }

    return 0;
}
