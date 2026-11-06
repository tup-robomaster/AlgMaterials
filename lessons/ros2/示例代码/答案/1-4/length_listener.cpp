// 练习 1-4 的答案：订阅字符串并打印长度。
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class LengthListener : public rclcpp::Node
{
public:
  LengthListener()
  : Node("length_listener"), count_(0)
  {
    subscription_ = create_subscription<std_msgs::msg::String>(
      "words", 10, [this](const std_msgs::msg::String::SharedPtr message) {
        RCLCPP_INFO(get_logger(), "length: %zu", message->data.size());
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
  rclcpp::spin(std::make_shared<LengthListener>());
  rclcpp::shutdown();
  return 0;
}
