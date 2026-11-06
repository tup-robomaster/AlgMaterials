// 1.5 的订阅者：只订阅 /robot1/chatter，不处理 /robot2/chatter。
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class FilterListener : public rclcpp::Node
{
public:
  FilterListener()
  : Node("filter_listener"), count_(0)
  {
    subscription_ = create_subscription<std_msgs::msg::String>(
      "/robot1/chatter", 10,
      [this](const std_msgs::msg::String::SharedPtr message) {
        RCLCPP_INFO(get_logger(), "from robot1: '%s'", message->data.c_str());
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
  rclcpp::spin(std::make_shared<FilterListener>());
  rclcpp::shutdown();
  return 0;
}
