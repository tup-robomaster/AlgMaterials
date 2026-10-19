// 6.1：从文件里读姓名和分数，逐个输出，再算平均分。
#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream in("6.1-scores.txt");
    if (!in) {
        std::cout << "文件打不开" << std::endl;
        return 1;
    }

    std::string name;
    int score = 0;
    int count = 0;
    int sum = 0;
    while (in >> name >> score) {
        std::cout << name << " " << score << std::endl;
        count++;
        sum = sum + score;
    }

    std::cout << "共 " << count << " 人，平均 " << sum / double(count) << std::endl;

    return 0;
}
