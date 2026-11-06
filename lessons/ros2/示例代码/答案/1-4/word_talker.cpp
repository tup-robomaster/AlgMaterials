// 练习 1-4 的辅助发布者：依次发布三个长度不同的单词。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class WordTalker : public rclcpp::Node
{
public:
  WordTalker()
  : Node("word_talker"), count_(0), started_(false)
  {
    publisher_ = create_publisher<std_msgs::msg::String>("words", 10);
    timer_ = create_wall_timer(300ms, [this]() {
      if (!started_ && publisher_->get_subscription_count() == 0) {
        return;
      }
      started_ = true;
      const char * words[3] = {"ros", "robot", "navigation"};
      std_msgs::msg::String message;
      message.data = words[count_];
      publisher_->publish(message);
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
  bool started_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<WordTalker>());
  rclcpp::shutdown();
  return 0;
}
