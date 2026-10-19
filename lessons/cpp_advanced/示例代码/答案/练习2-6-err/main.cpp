#include "stats.h"

int main() {
    std::vector<int> scores = readScores(10);

    printReport(scores);
    return 0;
}
