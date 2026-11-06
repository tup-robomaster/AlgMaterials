// 练习 0-4 的答案：日志中同时打印节点名与计数。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class TickNode : public rclcpp::Node
{
public:
  TickNode()
  : Node("tick_node"), count_(0)
  {
    timer_ = create_wall_timer(500ms, [this]() {
      RCLCPP_INFO(get_logger(), "%s %d", get_name(), count_);
      ++count_;
      if (count_ == 3) {
        rclcpp::shutdown();
      }
    });
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  int count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TickNode>());
  rclcpp::shutdown();
  return 0;
}
