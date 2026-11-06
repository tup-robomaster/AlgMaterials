// 练习 1-11 的辅助发布者：反复发布同一个字段，供 echo --once 取一条。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class ProbeTalker : public rclcpp::Node
{
public:
  ProbeTalker()
  : Node("probe_talker"), count_(0)
  {
    publisher_ = create_publisher<std_msgs::msg::String>("chatter", 10);
    timer_ = create_wall_timer(100ms, [this]() {
      if (publisher_->get_subscription_count() == 0) {
        return;
      }
      std_msgs::msg::String message;
      message.data = "probe";
      publisher_->publish(message);
      ++count_;
      if (count_ == 20) {
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
  rclcpp::spin(std::make_shared<ProbeTalker>());
  rclcpp::shutdown();
  return 0;
}
