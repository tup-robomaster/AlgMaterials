// 3.3：范围 for 里那个变量，也能带 const。
//
// 3.1 说了加 & 就不抄。可要是既不想抄、又不打算改呢？
// 那就把这两句一起写上：const + &。
//
// 地址每次运行都不一样，看的是「跟 names[0] 相同的那一行」。
//
// 运行一下：g++ -std=c++17 -Wall 示例代码/3.3-range-for-const.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>
#include <vector>

int main() {
    std::vector<std::string> names = {"小明", "小红", "小刚"};

    std::cout << "names[0] 的地址：" << &names[0] << std::endl;

    for (const std::string& s : names) {
        std::cout << s << " 的地址：" << &s << std::endl;
    }

    return 0;
}
