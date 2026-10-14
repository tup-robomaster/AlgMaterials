// 5.1 struct —— 反例：一个人拆成三个数组，靠下标对齐
//
// 想把「小明」和「小刚」换个位置（比如按成绩重排）。
// 名字那一排换了、成绩那一排也换了，学号那一排忘了动。
// 编译干净，跑起来也不报错，就是这三排从此对不上了。

#include <iostream>
#include <string>

int main() {
    std::string names[3] = {"小明", "小红", "小刚"};
    int ids[3] = {101, 102, 103};
    int scores[3] = {90, 85, 95};

    std::string t1 = names[0];
    names[0] = names[2];
    names[2] = t1;

    int t2 = scores[0];
    scores[0] = scores[2];
    scores[2] = t2;

    // ids[0] 和 ids[2] 忘了换

    for (int i = 0; i < 3; i++) {
        std::cout << names[i] << " " << ids[i] << " " << scores[i] << std::endl;
    }

    return 0;
}
