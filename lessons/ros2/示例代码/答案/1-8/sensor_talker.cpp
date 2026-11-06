// 练习 1-8 的发布者：两端都使用 SensorDataQoS，共发五条。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class SensorTalker : public rclcpp::Node
{
public:
  SensorTalker()
  : Node("sensor_talker"), count_(0), started_(false)
  {
    publisher_ = create_publisher<std_msgs::msg::String>(
      "sensor_chatter", rclcpp::SensorDataQoS());
    timer_ = create_wall_timer(200ms, [this]() {
      if (!started_ && publisher_->get_subscription_count() == 0) {
        return;
      }
      started_ = true;
      std_msgs::msg::String message;
      message.data = "sample " + std::to_string(count_);
      publisher_->publish(message);
      ++count_;
      if (count_ == 5) {
        rclcpp::shutdown();
      }
    });
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int count_;
  bool started_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SensorTalker>());
  rclcpp::shutdown();
  return 0;
}
