// 练习 0-8 的答案：新可执行文件 info_node，打印一行后退出。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class InfoNode : public rclcpp::Node
{
public:
  InfoNode()
  : Node("info_node")
  {
    timer_ = create_wall_timer(100ms, [this]() {
      RCLCPP_INFO(get_logger(), "robot ready");
      rclcpp::shutdown();
    });
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<InfoNode>());
  rclcpp::shutdown();
  return 0;
}
