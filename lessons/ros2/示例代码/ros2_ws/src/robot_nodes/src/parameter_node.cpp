// 2.4 的参数节点：声明 rate_ms 与 message，启动时打印，修改时打印回调。
#include <memory>
#include <string>
#include <vector>

#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include "rclcpp/rclcpp.hpp"

class ParameterNode : public rclcpp::Node
{
public:
  ParameterNode()
  : Node("parameter_node"), rate_ms_(500), message_("hello")
  {
    rate_ms_ = declare_parameter<int>("rate_ms", rate_ms_);
    message_ = declare_parameter<std::string>("message", message_);
    RCLCPP_INFO(
      get_logger(), "startup: rate_ms=%d message='%s'", rate_ms_, message_.c_str());

    callback_handle_ = add_on_set_parameters_callback(
      [this](const std::vector<rclcpp::Parameter> & parameters) {
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;
        for (const auto & parameter : parameters) {
          RCLCPP_INFO(
            get_logger(), "set: %s=%s",
            parameter.get_name().c_str(), parameter.value_to_string().c_str());
          if (parameter.get_name() == "rate_ms") {
            rate_ms_ = parameter.as_int();
          } else if (parameter.get_name() == "message") {
            message_ = parameter.as_string();
          }
        }
        return result;
      });
  }

private:
  int rate_ms_;
  std::string message_;
  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr callback_handle_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ParameterNode>());
  rclcpp::shutdown();
  return 0;
}
