// 0.2 的第一个节点：每 500 ms 打印一行，第三行之后自行停止。
// 停止是为了让例子能在终端里结束，页面上的输出可以逐行对照。
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
    timer_ = create_wall_timer(500ms, [this]() {
      RCLCPP_INFO(get_logger(), "hello %d", count_);
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
  rclcpp::spin(std::make_shared<HelloNode>());
  rclcpp::shutdown();
  return 0;
}
