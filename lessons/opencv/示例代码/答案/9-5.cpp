#include <iostream>
#include <string>
#include <vector>

int main() {
    const std::vector<int> detections = {1, 1, 1, 0, 0, 0,
                                         0, 0, 1, 1, 1, 1};
    bool confirmed = true;
    bool lost = false;
    int misses = 0;
    int recapture = 0;
    int transitions = 0;
    std::vector<std::string> states;

    for (int detected : detections) {
        if (detected) {
            ++recapture;
            if (lost && recapture >= 3) {
                lost = false;
                confirmed = true;
                recapture = 0;
                ++transitions;
            }
            misses = 0;
        } else {
            ++misses;
            recapture = 0;
            if (confirmed && misses >= 5) {
                confirmed = false;
                lost = true;
                ++transitions;
                misses = 0;
            }
        }
        states.push_back(lost ? "lost" : "confirmed");
    }

    std::cout << "frames = " << states.size() << '\n';
    for (std::size_t i = 0; i < states.size(); ++i) {
        std::cout << "state[" << i << "] = " << states[i] << '\n';
    }
    std::cout << "transitions = " << transitions << '\n';
    return 0;
}
