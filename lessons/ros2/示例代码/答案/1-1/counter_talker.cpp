// 练习 1-1 的答案：在 counter 上发布 0、1、2。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;

class CounterTalker : public rclcpp::Node
{
public:
  CounterTalker()
  : Node("counter_talker"), count_(0)
  {
    publisher_ = create_publisher<std_msgs::msg::Int32>("counter", 10);
    timer_ = create_wall_timer(500ms, [this]() {
      std_msgs::msg::Int32 message;
      message.data = count_;
      publisher_->publish(message);
      RCLCPP_INFO(get_logger(), "counter: %d", message.data);
      ++count_;
      if (count_ == 3) {
        rclcpp::shutdown();
      }
    });
  }

private:
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CounterTalker>());
  rclcpp::shutdown();
  return 0;
}
