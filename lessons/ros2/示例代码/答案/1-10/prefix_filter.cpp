// 练习 1-10 的答案：只处理以 target- 开头的字符串。
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class PrefixFilter : public rclcpp::Node
{
public:
  PrefixFilter()
  : Node("prefix_filter"), count_(0)
  {
    subscription_ = create_subscription<std_msgs::msg::String>(
      "chatter", 10, [this](const std_msgs::msg::String::SharedPtr message) {
        const std::string prefix = "target-";
        if (message->data.compare(0, prefix.size(), prefix) != 0) {
          return;
        }
        RCLCPP_INFO(get_logger(), "target: '%s'", message->data.c_str());
        ++count_;
        if (count_ == 2) {
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
  rclcpp::spin(std::make_shared<PrefixFilter>());
  rclcpp::shutdown();
  return 0;
}
