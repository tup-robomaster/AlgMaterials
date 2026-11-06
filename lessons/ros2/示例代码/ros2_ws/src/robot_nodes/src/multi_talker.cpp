// 1.5 的多发布者示例：从命令行取一个编号，拼进消息内容。
// 两个实例放在不同命名空间里，话题的完全限定名随之不同。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class MultiTalker : public rclcpp::Node
{
public:
  explicit MultiTalker(const std::string & id, int count)
  : Node("multi_talker"), id_(id), count_(0), count_limit_(count)
  {
    publisher_ = create_publisher<std_msgs::msg::String>("chatter", 10);
    timer_ = create_wall_timer(500ms, [this]() {
      std_msgs::msg::String message;
      message.data = id_ + "-" + std::to_string(count_);
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
  std::string id_;
  int count_;
  int count_limit_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  const auto args = rclcpp::remove_ros_arguments(argc, argv);
  const std::string id = args.size() > 1 ? args[1] : "robot";
  const int count = args.size() > 2 ? std::stoi(args[2]) : 3;
  rclcpp::spin(std::make_shared<MultiTalker>(id, count));
  rclcpp::shutdown();
  return 0;
}
