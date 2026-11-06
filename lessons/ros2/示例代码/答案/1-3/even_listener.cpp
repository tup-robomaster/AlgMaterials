// 练习 1-3 的答案：订阅整数，只打印偶数。
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

class EvenListener : public rclcpp::Node
{
public:
  EvenListener()
  : Node("even_listener"), count_(0)
  {
    subscription_ = create_subscription<std_msgs::msg::Int32>(
      "numbers", 10, [this](const std_msgs::msg::Int32::SharedPtr message) {
        if (message->data % 2 != 0) {
          return;
        }
        RCLCPP_INFO(get_logger(), "even: %d", message->data);
        ++count_;
        if (count_ == 5) {
          rclcpp::shutdown();
        }
      });
  }

private:
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_;
  int count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<EvenListener>());
  rclcpp::shutdown();
  return 0;
}
