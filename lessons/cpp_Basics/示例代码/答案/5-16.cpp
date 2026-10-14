// 练习 5-16 的答案：经理的年薪在父类那份上再加奖金。
//
// Manager::annualSalary() 里那句 Employee::annualSalary() 是
// 把父类的算法原样用一遍，再往上加——不重抄那十二个月的乘法。
// 以后基本工资怎么算要改，只改 Employee 那一处就行。

#include <iostream>
#include <string>

class Employee {
public:
    std::string name;
    int monthly;

    Employee(std::string name, int monthly) {
        this->name = name;
        this->monthly = monthly;
    }

    int annualSalary() {
        return monthly * 12;
    }
};

class Manager : public Employee {
public:
    int bonus;

    Manager(std::string name, int monthly, int bonus)
        : Employee(name, monthly) {
        this->bonus = bonus;
    }

    int annualSalary() {
        return Employee::annualSalary() + bonus;
    }
};

int main() {
    Employee e("小明", 8000);
    std::cout << e.annualSalary() << std::endl;

    Manager m("小红", 8000, 50000);
    std::cout << m.annualSalary() << std::endl;

    return 0;
}
