// 练习 1-7 的订阅者：晚加入，transient_local 让它收到最后一条。
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class LateListener : public rclcpp::Node
{
public:
  LateListener()
  : Node("late_listener")
  {
    subscription_ = create_subscription<std_msgs::msg::String>(
      "late_chatter", rclcpp::QoS(1).transient_local(),
      [this](const std_msgs::msg::String::SharedPtr message) {
        RCLCPP_INFO(get_logger(), "late: '%s'", message->data.c_str());
        rclcpp::shutdown();
      });
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LateListener>());
  rclcpp::shutdown();
  return 0;
}
