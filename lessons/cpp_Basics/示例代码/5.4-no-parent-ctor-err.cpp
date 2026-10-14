#include <iostream>
#include <string>

class Student {
public:
    std::string name;
    int id;
    int score;

    Student(std::string n, int i, int s) {
        name = n;
        id = i;
        score = s;
    }
};

class GradStudent : public Student {
public:
    std::string advisor;

    GradStudent(std::string n, int i, int s, std::string adv) {
        name = n;
        id = i;
        score = s;
        advisor = adv;
    }
};

int main() {
    GradStudent b("小红", 102, 85, "王老师");

    std::cout << b.name << " " << b.id << " " << b.score << " " << b.advisor << std::endl;

    return 0;
}
