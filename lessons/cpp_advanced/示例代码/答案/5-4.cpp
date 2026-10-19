// 练习 5-4 的答案：谁申请谁释放——函数负责申请，释放的工作交给调用它的人。
//
// 这一份里要紧的不在代码，而在那条约定：取得的内存，用完要释放。
// 申请和释放写在不同的人手里，就必须有这条约定作为依据——
// 哪天看到一份程序里 make_zeros 了却没配 delete[]，缺的就是这条约定。
//
// 5.4 的 unique_ptr 就是把这个约定写进类型里，不用再靠记忆。
//
// 顺带说明返回值那一行：data 是函数里申请的，可它不由这个函数释放——
// 返回的是那块内存的地址，内存本身仍然有效。

#include <iostream>

int* make_zeros(int n) {
    int* data = new int[n];
    for (int i = 0; i < n; i++) {
        data[i] = 0;
    }
    return data;
}

int main() {
    int* data = make_zeros(5);

    data[2] = 7;        // 取得后直接使用

    for (int i = 0; i < 5; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;

    delete[] data;      // 用完了，谁取得的谁释放
    data = nullptr;

    return 0;
}
