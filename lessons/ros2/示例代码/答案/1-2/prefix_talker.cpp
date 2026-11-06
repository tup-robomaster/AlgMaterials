// 练习 1-2 的答案：发布带固定前缀 target- 的字符串。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class PrefixTalker : public rclcpp::Node
{
public:
  PrefixTalker()
  : Node("prefix_talker"), count_(0)
  {
    publisher_ = create_publisher<std_msgs::msg::String>("chatter", 10);
    timer_ = create_wall_timer(500ms, [this]() {
      std_msgs::msg::String message;
      message.data = "target-" + std::to_string(count_);
      publisher_->publish(message);
      RCLCPP_INFO(get_logger(), "publishing: '%s'", message.data.c_str());
      ++count_;
      if (count_ == 3) {
        rclcpp::shutdown();
      }
    });
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PrefixTalker>());
  rclcpp::shutdown();
  return 0;
}
