// 10-3 的服务端：保存最新一条读数，按请求的系数换算后返回。
#include <memory>
#include <string>

#include "answer_interfaces/msg/reading.hpp"
#include "answer_interfaces/srv/scale.hpp"
#include "rclcpp/rclcpp.hpp"

class ScaleServer : public rclcpp::Node
{
public:
  ScaleServer()
  : Node("scale_server"), has_reading_(false)
  {
    subscription_ = create_subscription<answer_interfaces::msg::Reading>(
      "reading", 10,
      [this](const answer_interfaces::msg::Reading::SharedPtr message) {
        latest_ = *message;
        has_reading_ = true;
        RCLCPP_INFO(
          get_logger(), "latest: %s %.1f #%d",
          latest_.sensor.c_str(), latest_.value, latest_.sequence);
      });

    service_ = create_service<answer_interfaces::srv::Scale>(
      "scale",
      [this](
        const std::shared_ptr<answer_interfaces::srv::Scale::Request> request,
        std::shared_ptr<answer_interfaces::srv::Scale::Response> response) {
        if (!has_reading_) {
          response->ok = false;
          RCLCPP_INFO(get_logger(), "scale: no reading yet");
          return;
        }
        response->value = latest_.value * request->factor;
        response->ok = true;
        RCLCPP_INFO(
          get_logger(), "scale: %s %.1f * %.1f = %.1f ok=true",
          latest_.sensor.c_str(), latest_.value, request->factor,
          response->value);
      });
  }

private:
  bool has_reading_;
  answer_interfaces::msg::Reading latest_;
  rclcpp::Subscription<answer_interfaces::msg::Reading>::SharedPtr subscription_;
  rclcpp::Service<answer_interfaces::srv::Scale>::SharedPtr service_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ScaleServer>());
  rclcpp::shutdown();
  return 0;
}
