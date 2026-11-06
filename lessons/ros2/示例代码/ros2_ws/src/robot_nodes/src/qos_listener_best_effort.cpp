// 1.4 的对照订阅者：只把可靠性改成 best_effort，其余与 qos_listener 相同。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class QosListenerBestEffort : public rclcpp::Node
{
public:
  QosListenerBestEffort()
  : Node("qos_listener_best_effort"), count_(0), ticks_(0)
  {
    subscription_ = create_subscription<std_msgs::msg::String>(
      "qos_chatter", rclcpp::QoS(10).best_effort(),
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
  rclcpp::spin(std::make_shared<QosListenerBestEffort>());
  rclcpp::shutdown();
  return 0;
}
