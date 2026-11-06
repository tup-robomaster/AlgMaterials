// 练习 1-5 的答案：使用增加 confidence 字段后的 Target。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/msg/target.hpp"

using namespace std::chrono_literals;

class ConfidenceTalker : public rclcpp::Node
{
public:
  ConfidenceTalker()
  : Node("confidence_talker"), count_(0)
  {
    publisher_ = create_publisher<robot_interfaces::msg::Target>("target", 10);
    timer_ = create_wall_timer(500ms, [this]() {
      const char * labels[3] = {"dock", "aisle", "exit"};
      const double scores[3] = {0.90, 0.75, 0.60};

      robot_interfaces::msg::Target target;
      target.x = 1.0 + count_;
      target.y = 2.0;
      target.label = labels[count_];
      target.confidence = scores[count_];

      publisher_->publish(target);
      RCLCPP_INFO(
        get_logger(), "label=%s confidence=%.2f",
        target.label.c_str(), target.confidence);
      ++count_;
      if (count_ == 3) {
        rclcpp::shutdown();
      }
    });
  }

private:
  rclcpp::Publisher<robot_interfaces::msg::Target>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ConfidenceTalker>());
  rclcpp::shutdown();
  return 0;
}
