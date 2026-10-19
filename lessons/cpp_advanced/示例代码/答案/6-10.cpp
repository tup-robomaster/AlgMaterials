// 练习 6-10：综合。文件缺失抛自定义异常，数量不符抛 runtime_error。
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

class MissingFile : public std::runtime_error {
public:
    explicit MissingFile(const std::string& name)
        : std::runtime_error("找不到文件：" + name) {}
};

int main() {
    int n = 0;
    int count = 0;
    int sum = 0;

    try {
        std::ifstream in("6-10-in.txt");
        if (!in) {
            throw MissingFile("6-10-in.txt");
        }

        in >> n;
        int x = 0;
        while (in >> x) {
            count++;
            sum = sum + x;
        }

        if (count != n) {
            throw std::runtime_error("数量不符");
        }

        std::cout << "共 " << n << " 个，平均 " << double(sum) / n << std::endl;
    } catch (const MissingFile& e) {
        std::cout << "文件错误：" << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << "数据错误：" << e.what()
                  << "（文件写的是 " << n << " 个，实际读到 " << count << " 个）"
                  << std::endl;
    }

    return 0;
}
