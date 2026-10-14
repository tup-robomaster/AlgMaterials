#include <iostream>
#include <string>

class Student {
    std::string name;
    int id;
    int score;
};

int main() {
    Student a;

    a.name = "小明";

    std::cout << a.name << std::endl;

    return 0;
}
