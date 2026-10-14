#include <iostream>

int main() {
    int total = 0;

    {
        int n = 5;
        total = total + n;
    }

    std::cout << "外面：total = " << total << std::endl;
    std::cout << "外面：n = " << n << std::endl;
    return 0;
}
