// 1.4 的 QoS 订阅者：可靠性为 reliable，与 qos_talker 不兼容。
// 计时器每秒检查一次，第三秒时打印总数并停止，便于观察“一条也没收到”。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class QosListener : public rclcpp::Node
{
public:
  QosListener()
  : Node("qos_listener"), count_(0), ticks_(0)
  {
    subscription_ = create_subscription<std_msgs::msg::String>(
      "qos_chatter", rclcpp::QoS(10).reliable(),
      [this](const std_msgs::msg::String::SharedPtr message) {
        RCLCPP_INFO(get_logger(), "I heard: '%s'", message->data.c_str());
        ++count_;
      });
    timer_ = create_wall_timer(1s, [this]() {
      ++ticks_;
      if (ticks_ == 3) {
        RCLCPP_INFO(get_logger(), "total: %d messages", count_);
        rclcpp::shutdown();
      }
    });
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
  int count_;
  int ticks_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<QosListener>());
  rclcpp::shutdown();
  return 0;
}
