// 5.5 的可组合节点：与 talker 做同样的事，但编成共享库，
// 由组件容器在运行时加载，而不是作为独立进程启动。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

namespace robot_nodes
{

class TalkerComponent : public rclcpp::Node
{
public:
  explicit TalkerComponent(const rclcpp::NodeOptions & options)
  : Node("talker_component", options), count_(0)
  {
    publisher_ = create_publisher<std_msgs::msg::String>("component_chatter", 10);
    timer_ = create_wall_timer(500ms, [this]() {
      std_msgs::msg::String message;
      message.data = "component " + std::to_string(count_);
      publisher_->publish(message);
      ++count_;
    });
  }

private:
  int count_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

RCLCPP_COMPONENTS_REGISTER_NODE(robot_nodes::TalkerComponent)

}  // namespace robot_nodes
