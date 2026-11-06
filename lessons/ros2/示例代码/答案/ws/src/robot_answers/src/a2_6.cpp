// 2-6 的答案：矩形面积服务加入输入校验，非法尺寸返回错误字符串。
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/srv/compute_area.hpp"

class CheckedAreaServer : public rclcpp::Node
{
public:
  CheckedAreaServer()
  : Node("checked_area_server")
  {
    service_ = create_service<robot_interfaces::srv::ComputeArea>(
      "compute_area",
      [this](
        const std::shared_ptr<robot_interfaces::srv::ComputeArea::Request> request,
        std::shared_ptr<robot_interfaces::srv::ComputeArea::Response> response) {
        if (request->width < 0.0 || request->height < 0.0) {
          response->area = 0.0;
          response->message = "size must be non-negative";
        } else if (request->width == 0.0 || request->height == 0.0) {
          response->area = 0.0;
          response->message = "zero area";
        } else {
          response->area = request->width * request->height;
          response->message = "ok";
        }
        RCLCPP_INFO(
          get_logger(), "request: %.2f x %.2f", request->width, request->height);
        RCLCPP_INFO(
          get_logger(), "response: area=%.2f message=%s",
          response->area, response->message.c_str());
      });
  }

private:
  rclcpp::Service<robot_interfaces::srv::ComputeArea>::SharedPtr service_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CheckedAreaServer>());
  rclcpp::shutdown();
  return 0;
}
