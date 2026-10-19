// 5.2 例题（故意写错）：同一块内存释放两次。
//
// 这一份运行出来就会崩溃——运行库发现「这块内存刚才不是已经释放过了吗」，
// 立刻把程序中止。课件上引用的那两行就是它实际输出的内容。
// 在别处引用这段报错的时候，路径那一截（可执行文件叫什么名字）按自己的写。
//
// 编译并运行：g++ -std=c++17 -Wall 示例代码/5.2-double-free.cpp -o /tmp/t && /tmp/t

#include <iostream>

int main() {
    int* p = new int;
    *p = 7;
    std::cout << "查看：" << *p << std::endl;

    delete p;
    std::cout << "释放完成。" << std::endl;

    delete p;       // 同一块内存释放第二次——错在这一行
    std::cout << "这行到不了。" << std::endl;

    return 0;
}
