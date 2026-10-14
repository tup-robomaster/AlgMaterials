// 3.2：return 是「把结果交回去，然后当场走人」。
//
// 注意 doubleIt 里 return 底下那行 cout——它在程序里，但它一次都没跑过。
// 跑一遍看输出里有没有那句话，比背结论管用。

#include <iostream>

int doubleIt(int x) {
    return x * 2;
    std::cout << "这一行永远打不出来" << std::endl;
}

int main() {
    std::cout << doubleIt(3) << std::endl;
    std::cout << doubleIt(10) << std::endl;
    return 0;
}
