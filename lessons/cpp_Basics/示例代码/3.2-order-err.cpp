#include <iostream>

int main() {
    printTriangle(2);
    return 0;
}

void printTriangle(int n) {
    for (int i = 1; i <= n; i = i + 1) {
        for (int j = 1; j <= i; j = j + 1) {
            std::cout << "*";
        }
        std::cout << std::endl;
    }
}
