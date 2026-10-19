#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

int main() {
    std::map<std::string, int> counts;
    std::string word;
    while (std::cin >> word) {
        counts[word]++;
    }

    std::vector<std::pair<std::string, int>> items(counts.begin(), counts.end());
    std::sort(items.begin(), items.end(),
              [](const std::pair<std::string, int>& a,
                 const std::pair<std::string, int>& b) {
                  if (a.second != b.second) {
                      return a.second > b.second;
                  }
                  return a.first < b.first;
              });

    for (const auto& item : items) {
        std::cout << item.first << " " << item.second << "\n";
    }
    return 0;
}
