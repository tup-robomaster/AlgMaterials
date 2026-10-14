// 练习 6-13 的答案：个数得自己递进去
//
// 病根是 sizeof(a) 量的是指针不是数组。修法就是 4.x 一直在用的那个：
// 再收一个 n，个数由调用的人告诉它——所以 main 里传的是 3。

#include <iostream>

void printAll(int a[], int n) {
    for (int i = 0; i < n; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    int a[3] = {10, 20, 30};

    printAll(a, 3);

    return 0;
}
