// 2-7 的答案：声明 target_count 并打印默认值，打印后退出。
#include <memory>

#include "rclcpp/rclcpp.hpp"

class TargetCounter : public rclcpp::Node
{
public:
  TargetCounter()
  : Node("target_counter")
  {
    const int target_count = declare_parameter<int>("target_count", 3);
    RCLCPP_INFO(get_logger(), "target_count: %d", target_count);
  }
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TargetCounter>();
  rclcpp::shutdown();
  return 0;
}
