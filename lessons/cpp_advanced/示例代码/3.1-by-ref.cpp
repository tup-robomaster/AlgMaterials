// 3.1：vector 当参数递进函数，抄不抄？
//
// 跟基础课 6.4 那份 string 的地址对照方法相同，只是这次换成 vector。
// 地址每次运行都不一样，看的是「哪两行相同」。
//
// 运行一下：g++ -std=c++17 -Wall 示例代码/3.1-by-ref.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

void byValue(std::vector<int> v) {
    std::cout << "值传递   v 的地址：" << &v << std::endl;
    std::cout << "         元素个数：" << v.size() << " 个数" << std::endl;
}

void byRef(std::vector<int>& v) {
    v.push_back(100);
    std::cout << "引用传递 v 的地址：" << &v << std::endl;
    std::cout << "         元素个数：" << v.size() << " 个数" << std::endl;
}

int main() {
    std::vector<int> data = {90, 85, 77};

    std::cout << "main     data 自身的地址：" << &data << std::endl;

    byValue(data);
    std::cout << "值传递返回之后，main 里还有 "
              << data.size() << " 个数" << std::endl;

    byRef(data);
    std::cout << "引用传递返回之后，main 里还有 "
              << data.size() << " 个数" << std::endl;

    return 0;
}
