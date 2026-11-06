// 6.2 的生命周期发布者：配置时创建生命周期发布者，激活后按周期发布。
// 节点初始处于 unconfigured，配置、激活、停用、清理都由生命周期服务驱动。
#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "std_msgs/msg/string.hpp"

using CallbackReturn =
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class LifecycleTalker : public rclcpp_lifecycle::LifecycleNode
{
public:
  LifecycleTalker()
  : LifecycleNode("lifecycle_talker")
  {
    topic_ = declare_parameter<std::string>("topic", "chatter");
    period_ms_ = declare_parameter<int>("period_ms", 500);
    message_ = declare_parameter<std::string>("message", "active");
    fail_on_activate_ = declare_parameter<bool>("fail_on_activate", false);

    timer_ = create_wall_timer(
      std::chrono::milliseconds(period_ms_), [this]() {
        if (!publisher_ || !publisher_->is_activated()) {
          return;
        }
        std_msgs::msg::String message;
        message.data = message_;
        publisher_->publish(message);
      });
  }

  CallbackReturn on_configure(const rclcpp_lifecycle::State &) override
  {
    publisher_ = create_publisher<std_msgs::msg::String>(topic_, 10);
    RCLCPP_INFO(get_logger(), "configured: message='%s'", message_.c_str());
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State &) override
  {
    if (fail_on_activate_) {
      RCLCPP_ERROR(get_logger(), "activation refused");
      return CallbackReturn::FAILURE;
    }
    publisher_->on_activate();
    RCLCPP_INFO(get_logger(), "activated");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State &) override
  {
    publisher_->on_deactivate();
    RCLCPP_INFO(get_logger(), "deactivated");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State &) override
  {
    publisher_.reset();
    RCLCPP_INFO(get_logger(), "cleaned up");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State &) override
  {
    publisher_.reset();
    RCLCPP_INFO(get_logger(), "shut down");
    return CallbackReturn::SUCCESS;
  }

private:
  std::string topic_;
  int period_ms_;
  std::string message_;
  bool fail_on_activate_;
  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<LifecycleTalker>();
  rclcpp::spin(node->get_node_base_interface());
  rclcpp::shutdown();
  return 0;
}
