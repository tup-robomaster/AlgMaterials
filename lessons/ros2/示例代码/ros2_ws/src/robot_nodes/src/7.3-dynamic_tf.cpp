// 7.3 的动态坐标广播：odom 到 base_link 按固定步长沿 X 轴移动。
// 每一步都用当前时间重新写 header.stamp，走完预定步数后保持最后一个位姿。
#include <chrono>
#include <memory>
#include <string>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"

class DynamicTf : public rclcpp::Node
{
public:
  DynamicTf()
  : Node("dynamic_tf")
  {
    parent_ = declare_parameter<std::string>("parent_frame", "odom");
    child_ = declare_parameter<std::string>("child_frame", "base_link");
    steps_ = declare_parameter<int>("steps", 5);
    step_ = declare_parameter<double>("step", 0.5);
    const auto period_ms = declare_parameter<int>("period_ms", 500);

    broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
    timer_ = create_wall_timer(
      std::chrono::milliseconds(period_ms), [this]() { publish_step(); });
  }

private:
  void publish_step()
  {
    const int shown = index_ < steps_ ? index_ : steps_ - 1;

    geometry_msgs::msg::TransformStamped transform;
    transform.header.stamp = now();
    transform.header.frame_id = parent_;
    transform.child_frame_id = child_;
    transform.transform.translation.x = shown * step_;
    transform.transform.rotation.w = 1.0;
    broadcaster_->sendTransform(transform);
    if (index_ < steps_) {
      RCLCPP_INFO(get_logger(), "step %d: x=%.3f", shown, shown * step_);
      ++index_;
    }
  }

  std::string parent_;
  std::string child_;
  int steps_;
  double step_;
  int index_ = 0;
  std::unique_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DynamicTf>());
  rclcpp::shutdown();
  return 0;
}
