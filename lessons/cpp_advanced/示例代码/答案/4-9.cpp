// 练习 4-9 的答案：横线在第几个，就删第几个。
//
// 跟 4.3 正文里那份的区别只有一个：那个将 3 直接写入代码，
// 这个先拿 find 问出来——find 给的是位置，erase 要的正好也是位置，
// 两个接得上。
// 没问之前先判一下 npos：输入里可能压根没有横线，
// 那时候 find 给的是一大串「没有这个位置」的记号，
// 拿它去 erase 就会出错。

#include <iostream>
#include <string>

int main() {
    std::string phone;
    std::cin >> phone;

    std::cout << "原来：" << phone << std::endl;

    if (phone.find('-') == std::string::npos) {
        std::cout << "这串里没有横线" << std::endl;
        return 0;
    }

    int pos = int(phone.find('-'));
    std::cout << "横线在第 " << pos << " 个" << std::endl;

    phone.erase(pos, 1);

    std::cout << "去掉横线：" << phone << std::endl;

    return 0;
}
