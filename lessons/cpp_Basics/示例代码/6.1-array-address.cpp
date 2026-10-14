// 6.1 数组元素是挨着放的
//
// 三个元素的地址一个比一个大，而且每次正好大 4——
// 因为一个 int 占 4 个字节，它们背靠背。
//
// 地址每次跑都不一样，但「相差 4」这件事每次都一样。

#include <iostream>

int main() {
    int a[3] = {10, 20, 30};

    std::cout << &a[0] << std::endl;
    std::cout << &a[1] << std::endl;
    std::cout << &a[2] << std::endl;

    return 0;
}
