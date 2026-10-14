#include <iostream>

void printStars(int n) {
    for (int i = 1; i <= n; i = i + 1) {
        std::cout << "*";
    }
    std::cout << std::endl;
}

int main() {
    printStars();
    return 0;
}
