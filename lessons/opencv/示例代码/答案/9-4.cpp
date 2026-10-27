#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    const std::vector<char> digits = {'1', '2', '3', '4', '5'};
    const std::vector<double> scores = {0.5285, 0.4871, 0.8724,
                                        0.3352, 0.7193};
    int best = 0;
    int second = 1;
    for (int i = 1; i < 5; ++i) {
        if (scores[i] > scores[best]) {
            second = best;
            best = i;
        } else if (i != best && scores[i] > scores[second]) {
            second = i;
        }
    }
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "best_digit = " << digits[best] << '\n';
    std::cout << "confidence = " << scores[best] << '\n';
    std::cout << "margin = " << scores[best] - scores[second] << '\n';
    return 0;
}
