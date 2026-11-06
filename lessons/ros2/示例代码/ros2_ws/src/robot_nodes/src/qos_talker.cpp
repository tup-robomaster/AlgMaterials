// 1.4 的 QoS 发布者：可靠性为 best_effort，共发三条。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class QosTalker : public rclcpp::Node
{
public:
  QosTalker()
  : Node("qos_talker"), count_(0)
  {
    publisher_ = create_publisher<std_msgs::msg::String>(
      "qos_chatter", rclcpp::QoS(10).best_effort());
    timer_ = create_wall_timer(500ms, [this]() {
      std_msgs::msg::String message;
      message.data = "qos " + std::to_string(count_);
      publisher_->publish(message);
      RCLCPP_INFO(get_logger(), "sent: '%s'", message.data.c_str());
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
  rclcpp::spin(std::make_shared<QosTalker>());
  rclcpp::shutdown();
  return 0;
}
