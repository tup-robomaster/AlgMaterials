// 7.5 的时间查询：向同一个缓存写入两个时刻的变换，分别查询最新、插值与缓存之外。
// 只使用缓存，不建立广播者或订阅者，输出因此与网络时序无关。
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.h"
#include "tf2/time.h"
#include "tf2_ros/buffer.h"

namespace
{
geometry_msgs::msg::TransformStamped make_transform(double seconds, double x)
{
  geometry_msgs::msg::TransformStamped transform;
  transform.header.stamp = rclcpp::Time(static_cast<int64_t>(seconds), 0);
  transform.header.frame_id = "base_link";
  transform.child_frame_id = "marker";
  transform.transform.translation.x = x;
  transform.transform.rotation.w = 1.0;
  return transform;
}

double x_of(const tf2_ros::Buffer & buffer, double seconds)
{
  const auto transform = buffer.lookupTransform(
    "base_link", "marker", tf2::timeFromSec(seconds));
  return transform.transform.translation.x;
}
}  // namespace

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto clock = std::make_shared<rclcpp::Clock>(RCL_ROS_TIME);
  tf2_ros::Buffer buffer(clock);
  buffer.setTransform(make_transform(100.0, 1.0), "course");
  buffer.setTransform(make_transform(102.0, 3.0), "course");

  const std::string mode = argc > 1 ? argv[1] : "all";
  if (mode == "all" || mode == "latest") {
    const auto latest = buffer.lookupTransform(
      "base_link", "marker", tf2::TimePointZero);
    std::cout << "latest: x=" << std::fixed << std::setprecision(3)
              << latest.transform.translation.x << "\n";
  }
  if (mode == "all" || mode == "interp") {
    std::cout << "interpolated at 101s: x=" << std::fixed << std::setprecision(3)
              << x_of(buffer, 101.0) << "\n";
  }
  if (mode == "all" || mode == "outside") {
    try {
      x_of(buffer, 80.0);
      std::cout << "outside: unexpectedly succeeded\n";
    } catch (const tf2::TransformException & exception) {
      std::cout << "outside: " << exception.what() << "\n";
    }
  }
  rclcpp::shutdown();
  return 0;
}
