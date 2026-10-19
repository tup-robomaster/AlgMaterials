// 3.3：同一个 vector 过三道手，三种参数写法并排看。
//
// 基础课 6.4 用 string 讲过「较大的对象不要复制，复制了就不要修改」，这一份把该结论
// 应用到 vector 上，并且单独说明第三种（只读引用）：
// 它跟引用传递一样不复制，差别在于它承诺「我不修改」。
//
// 地址每次运行都不一样，看的是「哪两行相同」。
//
// 运行一下：g++ -std=c++17 -Wall 示例代码/3.3-const-param.cpp -o /tmp/t && /tmp/t

#include <iostream>
#include <vector>

void byValue(std::vector<int> v) {
    v.push_back(100);
    std::cout << "值传递   v 的地址：" << &v << "，元素个数："
              << v.size() << " 个数" << std::endl;
}

void byRef(std::vector<int>& v) {
    v.push_back(100);
    std::cout << "引用传递 v 的地址：" << &v << "，元素个数："
              << v.size() << " 个数" << std::endl;
}

void byConstRef(const std::vector<int>& v) {
    std::cout << "只读引用 v 的地址：" << &v << "，元素个数："
              << v.size() << " 个数" << std::endl;
}

int main() {
    std::vector<int> data = {90, 85, 77};

    std::cout << "main     data 的地址：" << &data << std::endl;

    byValue(data);
    std::cout << "         回到 main，元素个数："
              << data.size() << " 个数" << std::endl;

    byRef(data);
    std::cout << "         回到 main，元素个数："
              << data.size() << " 个数" << std::endl;

    byConstRef(data);
    std::cout << "         回到 main，元素个数："
              << data.size() << " 个数" << std::endl;

    return 0;
}
