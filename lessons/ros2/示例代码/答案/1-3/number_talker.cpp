// 练习 1-3 的辅助发布者：依次发布 0 到 9，供筛选端观察。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;

class NumberTalker : public rclcpp::Node
{
public:
  NumberTalker()
  : Node("number_talker"), count_(0), started_(false)
  {
    publisher_ = create_publisher<std_msgs::msg::Int32>("numbers", 10);
    timer_ = create_wall_timer(100ms, [this]() {
      if (!started_ && publisher_->get_subscription_count() == 0) {
        return;
      }
      started_ = true;
      std_msgs::msg::Int32 message;
      message.data = count_;
      publisher_->publish(message);
      ++count_;
      if (count_ == 10) {
        rclcpp::shutdown();
      }
    });
  }

private:
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int count_;
  bool started_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<NumberTalker>());
  rclcpp::shutdown();
  return 0;
}
