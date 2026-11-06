// 1.3 的自定义消息发布者：发布三条 Target，字段依次为坐标与标签。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/msg/target.hpp"

using namespace std::chrono_literals;

class TargetTalker : public rclcpp::Node
{
public:
  TargetTalker()
  : Node("target_talker"), count_(0)
  {
    publisher_ = create_publisher<robot_interfaces::msg::Target>("target", 10);
    timer_ = create_wall_timer(500ms, [this]() {
      const double coordinates[3][2] = {{1.0, 2.0}, {2.5, 3.5}, {4.0, 5.0}};
      const char * labels[3] = {"dock", "aisle", "exit"};

      robot_interfaces::msg::Target target;
      target.x = coordinates[count_][0];
      target.y = coordinates[count_][1];
      target.label = labels[count_];

      publisher_->publish(target);
      RCLCPP_INFO(
        get_logger(), "publishing: label=%s x=%.1f y=%.1f",
        target.label.c_str(), target.x, target.y);
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
  rclcpp::spin(std::make_shared<TargetTalker>());
  rclcpp::shutdown();
  return 0;
}
