// 练习 1-10 的辅助发布者：混发带 target- 前缀与不带前缀的字符串。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class PrefixWords : public rclcpp::Node
{
public:
  PrefixWords()
  : Node("prefix_words"), count_(0), started_(false)
  {
    publisher_ = create_publisher<std_msgs::msg::String>("chatter", 10);
    timer_ = create_wall_timer(300ms, [this]() {
      if (!started_ && publisher_->get_subscription_count() == 0) {
        return;
      }
      started_ = true;
      const char * words[4] = {"target-alpha", "noise", "target-beta", "noise 2"};
      std_msgs::msg::String message;
      message.data = words[count_];
      publisher_->publish(message);
      ++count_;
      if (count_ == 4) {
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
  rclcpp::spin(std::make_shared<PrefixWords>());
  rclcpp::shutdown();
  return 0;
}
