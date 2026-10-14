// 2.5：内圈一共转了几圈。
//
// 外圈 2 次、内圈 3 次，大括号里那句一共走了 2 乘 3 = 6 遍。
// 内圈那句「一共走了 6 遍」是必然的：外圈每转一次，内圈就转满一整轮。

#include <iostream>

int main() {
    int count = 0;

    for (int i = 1; i <= 2; i = i + 1) {
        for (int j = 1; j <= 3; j = j + 1) {
            count = count + 1;
        }
    }

    std::cout << "里面那句一共走了 " << count << " 遍" << std::endl;

    return 0;
}
