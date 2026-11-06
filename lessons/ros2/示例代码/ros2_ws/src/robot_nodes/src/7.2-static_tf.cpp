// 7.2 的静态坐标广播：把传感器安装位置作为固定变换发布一次。
// 平移、yaw 与坐标系名称都由参数给出，默认是 base_link 到 laser_link。
#include <cmath>
#include <memory>
#include <string>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/static_transform_broadcaster.h"

namespace
{
constexpr double kPi = 3.14159265358979323846;
}

class StaticTf : public rclcpp::Node
{
public:
  StaticTf()
  : Node("static_tf")
  {
    const auto parent = declare_parameter<std::string>("parent_frame", "base_link");
    const auto child = declare_parameter<std::string>("child_frame", "laser_link");
    const auto x = declare_parameter<double>("x", 0.2);
    const auto y = declare_parameter<double>("y", 0.0);
    const auto z = declare_parameter<double>("z", 0.3);
    const auto yaw_deg = declare_parameter<double>("yaw_deg", 45.0);

    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, yaw_deg * kPi / 180.0);

    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp = now();
    transform.header.frame_id = parent;
    transform.child_frame_id = child;
    transform.transform.translation.x = x;
    transform.transform.translation.y = y;
    transform.transform.translation.z = z;
    transform.transform.rotation.x = q.x();
    transform.transform.rotation.y = q.y();
    transform.transform.rotation.z = q.z();
    transform.transform.rotation.w = q.w();

    broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(*this);
    broadcaster_->sendTransform(transform);
    RCLCPP_INFO(
      get_logger(), "static transform %s -> %s", parent.c_str(), child.c_str());
  }

private:
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaster_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StaticTf>());
  rclcpp::shutdown();
  return 0;
}
