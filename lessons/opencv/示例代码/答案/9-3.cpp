#include <iomanip>
#include <iostream>

namespace {

struct Candidate {
    char name;
    double angle_error;
    double center_error;
    double length_ratio;
};

bool accepted(const Candidate& c) {
    return c.angle_error <= 8.0 && c.center_error <= 0.25 &&
           c.length_ratio >= 0.8;
}

double score(const Candidate& c) {
    return c.angle_error / 8.0 + c.center_error / 0.25 +
           (1.0 - c.length_ratio) / 0.2;
}

}  // namespace

int main() {
    const Candidate a{'A', 2.0, 0.05, 0.95};
    const Candidate b{'B', 5.0, 0.20, 0.82};
    std::cout << std::fixed << std::setprecision(3);
    for (const Candidate& c : {a, b}) {
        std::cout << "candidate " << c.name
                  << " accepted=" << (accepted(c) ? "yes" : "no")
                  << " score=" << score(c) << '\n';
    }
    std::cout << "best = " << (score(a) < score(b) ? 'A' : 'B') << '\n';
    return 0;
}
