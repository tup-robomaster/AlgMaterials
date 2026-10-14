// 3.3：两个同名盒子。
//
// 这叫「遮蔽」（shadow）。两个 n 是同名，但**不是同一个盒子**，
// 谁也不认识谁。函数里那个 n 变成多少，main 里这个一点都不受影响。
//
// 写代码时尽量别这么起名——人看着容易糊涂。知道有这回事就行：
// 名字是给同一块地盘里的人用的，出了这块地盘，它跟你没关系。

#include <iostream>

void show() {
    int n = 7;
    std::cout << "show 里 n = " << n << std::endl;
}

int main() {
    int n = 3;
    show();
    std::cout << "main 里 n = " << n << std::endl;
    return 0;
}
