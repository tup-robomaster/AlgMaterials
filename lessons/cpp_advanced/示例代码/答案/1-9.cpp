// 练习 1-9：把上一题那个「欠着的」补上，跑出 42。
//
// 补法是第二种：声明留在上面，函数体写在 main 下面。
// 第一种（把整个函数搬到 main 上面，基础课的老办法）也跑得通，
// 那样就没有下面这一问了。
//
// 「把声明那行删掉试试」的结果，真试过：
//   函数体留在 main 下面、声明一删，编译就不过了：
//       error: ‘twice’ was not declared in this scope
//   编译器自上往下读，读到 main 里那句 twice(21) 的时候，
//   它还没见过任何叫 twice 的东西，就只能报「不认识这个名字」。
//   （这条错和上一题那条 undefined reference 不是一回事：
//    一个是编译期「没听说过你」，一个是链接期「听说过了，但没人兑现」。）

#include <iostream>

int twice(int x);

int main() {
    std::cout << twice(21) << std::endl;
    return 0;
}

int twice(int x) {
    return x * 2;
}
