// 2.5 的参数文件节点：按 rate_ms 发布 message，修改参数后重建定时器。
#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class ParamDemo : public rclcpp::Node
{
public:
  ParamDemo()
  : Node("param_demo"), rate_ms_(500), message_("tick")
  {
    rate_ms_ = declare_parameter<int>("rate_ms", rate_ms_);
    message_ = declare_parameter<std::string>("message", message_);
    publisher_ = create_publisher<std_msgs::msg::String>("param_chatter", 10);
    timer_ = create_wall_timer(
      std::chrono::milliseconds(rate_ms_), [this]() { publish_tick(); });
    RCLCPP_INFO(
      get_logger(), "startup: rate_ms=%d message='%s'", rate_ms_, message_.c_str());

    callback_handle_ = add_on_set_parameters_callback(
      [this](const std::vector<rclcpp::Parameter> & parameters) {
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;
        for (const auto & parameter : parameters) {
          if (parameter.get_name() == "rate_ms") {
            rate_ms_ = parameter.as_int();
            timer_->cancel();
            timer_ = create_wall_timer(
              std::chrono::milliseconds(rate_ms_), [this]() { publish_tick(); });
            RCLCPP_INFO(get_logger(), "timer reset: rate_ms=%d", rate_ms_);
          } else if (parameter.get_name() == "message") {
            message_ = parameter.as_string();
            RCLCPP_INFO(get_logger(), "message: '%s'", message_.c_str());
          }
        }
        return result;
      });
  }

private:
  void publish_tick()
  {
    std_msgs::msg::String message;
    message.data = message_;
    publisher_->publish(message);
  }

  int rate_ms_;
  std::string message_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr callback_handle_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ParamDemo>());
  rclcpp::shutdown();
  return 0;
}
