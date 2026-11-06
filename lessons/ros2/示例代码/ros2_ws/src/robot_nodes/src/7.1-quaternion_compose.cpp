// 7.1 的例题：把两个绕 Z 轴的四元数相乘，比较乘积的角度与单位化结果。
// 不带参数时使用 30° 与 60°，带两个参数时按命令行给出的角度计算。
#include <cmath>
#include <iomanip>
#include <iostream>

#include "tf2/LinearMath/Quaternion.h"
#include "tf2/utils.h"

namespace
{
constexpr double kPi = 3.14159265358979323846;

tf2::Quaternion make_yaw(double degrees)
{
  tf2::Quaternion q;
  q.setRPY(0.0, 0.0, degrees * kPi / 180.0);
  return q;
}
}  // namespace

int main(int argc, char ** argv)
{
  const double first = argc > 1 ? std::stod(argv[1]) : 30.0;
  const double second = argc > 2 ? std::stod(argv[2]) : 60.0;

  tf2::Quaternion q = make_yaw(first) * make_yaw(second);
  std::cout << "q" << first << " * q" << second << ": x=" << std::fixed
            << std::setprecision(6) << q.x() << " y=" << q.y()
            << " z=" << q.z() << " w=" << q.w() << "\n";
  std::cout << "getYaw: " << tf2::getYaw(q) * 180.0 / kPi << "°\n";

  q.normalize();
  std::cout << "normalized norm: " << std::sqrt(q.x() * q.x() + q.y() * q.y() +
    q.z() * q.z() + q.w() * q.w()) << "\n";
  return 0;
}
