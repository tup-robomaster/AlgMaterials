// 1.3 的自定义消息订阅者：读取 Target 的三个字段并打印。
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/msg/target.hpp"

class TargetListener : public rclcpp::Node
{
public:
  TargetListener()
  : Node("target_listener"), count_(0)
  {
    subscription_ = create_subscription<robot_interfaces::msg::Target>(
      "target", 10, [this](const robot_interfaces::msg::Target::SharedPtr message) {
        RCLCPP_INFO(
          get_logger(), "target: label=%s x=%.1f y=%.1f",
          message->label.c_str(), message->x, message->y);
        ++count_;
        if (count_ == 3) {
          rclcpp::shutdown();
        }
      });
  }

private:
  rclcpp::Subscription<robot_interfaces::msg::Target>::SharedPtr subscription_;
  int count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TargetListener>());
  rclcpp::shutdown();
  return 0;
}
