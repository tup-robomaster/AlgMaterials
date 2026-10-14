// 6.1 例题：一排盒子占多少格。
//
// sizeof(整排) 是这一排总共占的格数，sizeof(一个元素) 是一个盒子的格数，
// 除一下就是这排有几个盒子。
// 最后两行打的是地址——每次都变，看的是它们差多少。

#include <iostream>

int main() {
    double price[4] = {9.9, 19.9, 29.9, 39.9};

    std::cout << "整排占几格    " << sizeof(price) << std::endl;
    std::cout << "一个盒子几格  " << sizeof(price[0]) << std::endl;
    std::cout << "一共几个盒子  " << sizeof(price) / sizeof(price[0]) << std::endl;

    std::cout << "第一个的地址  " << &price[0] << std::endl;
    std::cout << "第二个的地址  " << &price[1] << std::endl;

    return 0;
}
