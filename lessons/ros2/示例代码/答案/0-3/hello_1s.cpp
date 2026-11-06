// 练习 0-3 的答案：周期改为 1 s，打印五行后停止。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class HelloNode : public rclcpp::Node
{
public:
  HelloNode()
  : Node("hello_node"), count_(0)
  {
    timer_ = create_wall_timer(1s, [this]() {
      RCLCPP_INFO(get_logger(), "hello %d", count_);
      ++count_;
      if (count_ == 5) {
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
  rclcpp::spin(std::make_shared<HelloNode>());
  rclcpp::shutdown();
  return 0;
}
