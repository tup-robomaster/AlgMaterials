// 2-8 的答案：在参数回调中拒绝 target_count < 0 的修改。
#include <memory>
#include <vector>

#include "rcl_interfaces/msg/set_parameters_result.hpp"
#include "rclcpp/rclcpp.hpp"

class TargetCounter : public rclcpp::Node
{
public:
  TargetCounter()
  : Node("target_counter"), target_count_(3)
  {
    target_count_ = declare_parameter<int>("target_count", target_count_);
    RCLCPP_INFO(get_logger(), "target_count: %d", target_count_);

    callback_handle_ = add_on_set_parameters_callback(
      [this](const std::vector<rclcpp::Parameter> & parameters) {
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;
        for (const auto & parameter : parameters) {
          if (parameter.get_name() != "target_count") {
            continue;
          }
          if (parameter.as_int() < 0) {
            result.successful = false;
            result.reason = "target_count must be non-negative";
            RCLCPP_INFO(
              get_logger(), "rejected: target_count=%d", parameter.as_int());
          } else {
            target_count_ = parameter.as_int();
            RCLCPP_INFO(
              get_logger(), "accepted: target_count=%d", target_count_);
          }
        }
        return result;
      });
  }

private:
  int target_count_;
  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr callback_handle_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TargetCounter>());
  rclcpp::shutdown();
  return 0;
}
