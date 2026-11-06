// 1.2 的订阅者：订阅 chatter，打印收到的字符串，三条之后停止。
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Listener : public rclcpp::Node
{
public:
  Listener()
  : Node("listener"), count_(0)
  {
    subscription_ = create_subscription<std_msgs::msg::String>(
      "chatter", 10, [this](const std_msgs::msg::String::SharedPtr message) {
        RCLCPP_INFO(get_logger(), "I heard: '%s'", message->data.c_str());
        ++count_;
        if (count_ == 3) {
          rclcpp::shutdown();
        }
      });
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  int count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener>());
  rclcpp::shutdown();
  return 0;
}
