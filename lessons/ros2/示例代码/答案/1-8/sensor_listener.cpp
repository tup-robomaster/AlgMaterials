// 练习 1-8 的订阅者：使用 SensorDataQoS 计数，三秒后打印条数。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class SensorListener : public rclcpp::Node
{
public:
  SensorListener()
  : Node("sensor_listener"), count_(0)
  {
    subscription_ = create_subscription<std_msgs::msg::String>(
      "sensor_chatter", rclcpp::SensorDataQoS(),
      [this](const std_msgs::msg::String::SharedPtr) {
        ++count_;
      });
    timer_ = create_wall_timer(3s, [this]() {
      RCLCPP_INFO(get_logger(), "received: %d", count_);
      rclcpp::shutdown();
    });
  }

private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  rclcpp::TimerBase::SharedPtr timer_;
  int count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SensorListener>());
  rclcpp::shutdown();
  return 0;
}
