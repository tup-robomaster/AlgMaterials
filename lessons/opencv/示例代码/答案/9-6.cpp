#include <iomanip>
#include <iostream>

int main() {
    const int roi_x = 60;
    const int roi_y = 120;
    const double local_x = 368.578;
    const double local_y = 157.865;
    const double full_x = local_x + roi_x;
    const double full_y = local_y + roi_y;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "roi = (60, 120, 520, 300)\n";
    std::cout << "local_center = (" << local_x << ", " << local_y << ")\n";
    std::cout << "full_center = (" << full_x << ", " << full_y << ")\n";
    std::cout << "min_area = 500.000\n";
    return 0;
}
