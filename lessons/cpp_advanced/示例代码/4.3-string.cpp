// 4.3：string 也能这么删、这么插——它就是一堆字符装的容器。
//
// 3.2 学的 substr 是「剪一段出来给我」，原来的串一个字都不动；
// 这一课的 erase / insert 是在原来的串上直接动。
// 洗电话号码这种活（把横线去掉）就是它俩的拿手戏。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/4.3-string.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <string>

int main() {
    std::string phone = "138-0013-8000";

    for (auto it = phone.begin(); it != phone.end(); ) {
        if (*it == '-') {
            it = phone.erase(it);
        } else {
            ++it;
        }
    }

    std::cout << "去掉横线：" << phone << std::endl;

    phone.insert(phone.begin() + 3, '-');

    std::cout << "再插回去：" << phone << std::endl;

    return 0;
}
