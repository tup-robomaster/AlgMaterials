// 练习 1-7 的发布者：transient_local 保留最后一条，发布后仍存活一段时间。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class LateTalker : public rclcpp::Node
{
public:
  LateTalker()
  : Node("late_talker"), count_(0)
  {
    publisher_ = create_publisher<std_msgs::msg::String>(
      "late_chatter", rclcpp::QoS(1).transient_local());
    timer_ = create_wall_timer(500ms, [this]() {
      std_msgs::msg::String message;
      message.data = "late " + std::to_string(count_);
      publisher_->publish(message);
      RCLCPP_INFO(get_logger(), "published: '%s'", message.data.c_str());
      ++count_;
      if (count_ == 3) {
        timer_->cancel();
      }
    });
    stop_timer_ = create_wall_timer(3s, [this]() {
      rclcpp::shutdown();
    });
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::TimerBase::SharedPtr stop_timer_;
  int count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LateTalker>());
  rclcpp::shutdown();
  return 0;
}
