// 6.4 例题：一个函数带回两个结果。
//
// show 只是看看分数，名字是「大东西」，所以参数用 const& —— 不抄也不改。
// sumAndAvg 要把两个结果都留在外面，所以两个结果都用引用：
// 一个函数 return 只能带回来一样东西，剩下的一样得这么带。

#include <iostream>
#include <string>

void show(const std::string& name, int score) {
    std::cout << name << " 考了 " << score << " 分" << std::endl;
}

void sumAndAvg(int a, int b, int c, int& sum, double& avg) {
    sum = a + b + c;
    avg = sum / 3.0;
}

int main() {
    std::string name = "小明";
    int x = 90;
    int y = 85;
    int z = 77;

    show(name, x);

    int sum = 0;
    double avg = 0.0;

    sumAndAvg(x, y, z, sum, avg);

    std::cout << "总分 " << sum << std::endl;
    std::cout << "平均 " << avg << std::endl;

    return 0;
}
