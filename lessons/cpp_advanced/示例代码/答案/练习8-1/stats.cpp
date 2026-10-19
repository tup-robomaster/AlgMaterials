#include "stats.h"

int pass_count(const std::vector<int>& scores, int pass) {
    int count = 0;
    for (int s : scores) {
        if (s >= pass) {
            count++;
        }
    }
    return count;
}

double average(const std::vector<int>& scores) {
    int sum = 0;
    for (int s : scores) {
        sum += s;
    }
    return sum * 1.0 / scores.size();
}

int max_score(const std::vector<int>& scores) {
    int best = scores[0];
    for (int s : scores) {
        if (s > best) {
            best = s;
        }
    }
    return best;
}
