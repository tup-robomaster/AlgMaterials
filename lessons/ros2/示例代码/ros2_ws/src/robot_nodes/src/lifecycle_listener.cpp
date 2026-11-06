// 6.4 的生命周期订阅者：激活时才建立订阅，停用时撤销订阅。
// 这样未激活的节点不会出现在话题的订阅者计数里，也收不到消息。
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "std_msgs/msg/string.hpp"

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class LifecycleListener : public rclcpp_lifecycle::LifecycleNode
{
public:
  LifecycleListener()
  : LifecycleNode("lifecycle_listener")
  {
    topic_ = declare_parameter<std::string>("topic", "chatter");
  }

  CallbackReturn on_configure(const rclcpp_lifecycle::State &) override
  {
    RCLCPP_INFO(get_logger(), "configured");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State &) override
  {
    received_ = false;
    subscription_ = create_subscription<std_msgs::msg::String>(
      topic_, 10, [this](const std_msgs::msg::String::SharedPtr message) {
        if (received_) {
          return;
        }
        received_ = true;
        RCLCPP_INFO(get_logger(), "received: '%s'", message->data.c_str());
      });
    RCLCPP_INFO(get_logger(), "activated");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &) override
  {
    subscription_.reset();
    RCLCPP_INFO(get_logger(), "deactivated");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override
  {
    subscription_.reset();
    RCLCPP_INFO(get_logger(), "cleaned up");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &) override
  {
    subscription_.reset();
    RCLCPP_INFO(get_logger(), "shut down");
    return CallbackReturn::SUCCESS;
  }

private:
  std::string topic_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  bool received_ = false;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<LifecycleListener>();
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}
