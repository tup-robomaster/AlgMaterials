#include <iostream>
#include <string>

class Student {
private:
    std::string name;
    int score;

public:
    Student(std::string n, int s) {
        name = n;
        score = s;
    }

    int getScore() {
        return score;
    }
};

int main() {
    Student a("小明", 90);

    std::cout << a.getScore() << std::endl;

    a.score = 100;

    return 0;
}
