// 1.1 的发布者：每 500 ms 向 chatter 发布一条字符串，默认三条。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class Talker : public rclcpp::Node
{
public:
  explicit Talker(int count)
  : Node("talker"), count_(0), count_limit_(count)
  {
    wait_for_subscriber_ = declare_parameter<bool>("wait_for_subscriber", false);
    publisher_ = create_publisher<std_msgs::msg::String>("chatter", 10);
    timer_ = create_wall_timer(500ms, [this]() {
      if (wait_for_subscriber_ && publisher_->get_subscription_count() == 0) {
        return;
      }
      std_msgs::msg::String message;
      message.data = "hello " + std::to_string(count_);
      publisher_->publish(message);
      RCLCPP_INFO(get_logger(), "publishing: '%s'", message.data.c_str());
      ++count_;
      if (count_ == count_limit_) {
        rclcpp::shutdown();
      }
    });
  }

private:
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  bool wait_for_subscriber_{false};
  int count_;
  int count_limit_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  const auto args = rclcpp::remove_ros_arguments(argc, argv);
  const int count = args.size() > 1 ? std::stoi(args[1]) : 3;
  rclcpp::spin(std::make_shared<Talker>(count));
  rclcpp::shutdown();
  return 0;
}
