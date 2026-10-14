// 4.6：不写那个 int() 会怎样。
//
// 这份是**能跑的**——编译只是警告一句，程序照样出来、照样跑对。
// 名字里带 warn，就是说它故意留了这个警告，课里贴的就是编译器那句话。
//
// .size() 给出的数，类型跟 int 不是同一个（它是「装得下更大数的
// 那一种」）。两个不同型号的数放在一起比大小，编译器要提醒你一声。
// 前面加个 int() 把它变成 int，就没事了。

#include <iostream>
#include <vector>

int main() {
    std::vector<int> v;

    v.push_back(90);
    v.push_back(85);
    v.push_back(77);

    for (int i = 0; i < v.size(); i++) {
        std::cout << v[i] << std::endl;
    }

    return 0;
}
