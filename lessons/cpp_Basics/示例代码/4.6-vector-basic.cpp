// 4.6：vector——会自己变长的数组。
//
// 声明的时候不说几个，一个字都不说。要用的时候往里塞，
// 塞几个就有几个。
//
// 下标访问和数组一模一样：v[0] 是第一个。
// 遍历也一样，只有一个地方要留神——见下面那行注释。

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;                 // 一个都还没有

    v.push_back(90);                    // 塞进来一个，现在有 1 个
    v.push_back(85);                    // 2 个
    v.push_back(77);                    // 3 个

    std::cout << v.size() << std::endl; // 几个？问它
    std::cout << v[0] << std::endl;
    std::cout << v[2] << std::endl;

    v[2] = 88;                          // 改一个，和数组一样
    std::cout << v[2] << std::endl;

    for (int i = 0; i < int(v.size()); i++) {
        std::cout << v[i] << std::endl;
    }

    return 0;
}
