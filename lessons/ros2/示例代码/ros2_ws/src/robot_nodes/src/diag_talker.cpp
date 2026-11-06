// 1.6 的诊断用发布者：10 Hz 连发 50 条，供 topic hz 观察频率。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class DiagTalker : public rclcpp::Node
{
public:
  DiagTalker()
  : Node("diag_talker"), count_(0)
  {
    publisher_ = create_publisher<std_msgs::msg::String>("diag_chatter", 10);
    timer_ = create_wall_timer(100ms, [this]() {
      std_msgs::msg::String message;
      message.data = "diag " + std::to_string(count_);
      publisher_->publish(message);
      ++count_;
      if (count_ == 50) {
        RCLCPP_INFO(get_logger(), "published %d messages", count_);
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
  rclcpp::spin(std::make_shared<DiagTalker>());
  rclcpp::shutdown();
  return 0;
}
