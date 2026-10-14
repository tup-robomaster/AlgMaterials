// 练习 7-6（加打印）：让循环自己报出每一轮的分数和计数。
//
// 打印加在 if 那句后面：这一轮拿到的是几分、加完它之后 pass 是几。
// 病根是那一行的 >——它把「正好 60」挡在门外了，改成 >= 就对了。

#include <iostream>

int main() {
    int scores[5] = {60, 55, 72, 60, 48};
    int pass = 0;

    for (int i = 0; i < 5; i++) {
        if (scores[i] >= 60) {
            pass = pass + 1;
        }
        std::cout << "第 " << i << " 个是 " << scores[i]
                  << "，加完它之后 pass = " << pass << std::endl;
    }

    std::cout << "及格人数：" << pass << std::endl;

    return 0;
}
