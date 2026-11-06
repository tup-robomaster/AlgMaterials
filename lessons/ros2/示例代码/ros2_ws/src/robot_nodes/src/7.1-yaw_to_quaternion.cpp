// 7.1 的例题：把绕 Z 轴的 yaw 角换成四元数，再用 tf2::getYaw 还原。
// 不带参数时打印 90° 与 180°，带参数时按命令行给出的角度打印。
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include "tf2/LinearMath/Quaternion.h"
#include "tf2/utils.h"

namespace
{
constexpr double kPi = 3.14159265358979323846;

void print_yaw(double degrees)
{
  tf2::Quaternion q;
  q.setRPY(0.0, 0.0, degrees * kPi / 180.0);

  std::cout << "yaw " << std::defaultfloat << degrees << "°: x=" << std::fixed
            << std::setprecision(6) << q.x() << " y=" << q.y() << " z=" << q.z()
            << " w=" << q.w() << "\n";
  std::cout << "getYaw: " << tf2::getYaw(q) * 180.0 / kPi << "°\n";
}
}  // namespace

int main(int argc, char ** argv)
{
  std::vector<double> angles;
  for (int i = 1; i < argc; ++i) {
    angles.push_back(std::stod(argv[i]));
  }
  if (angles.empty()) {
    angles = {90.0, 180.0};
  }
  for (const double angle : angles) {
    print_yaw(angle);
  }
  return 0;
}
