// 2.3 的服务端：用自定义 ComputeArea 计算矩形面积，响应里带回消息。
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/srv/compute_area.hpp"

class AreaServer : public rclcpp::Node
{
public:
  AreaServer()
  : Node("area_server")
  {
    service_ = create_service<robot_interfaces::srv::ComputeArea>(
      "compute_area",
      [this](
        const std::shared_ptr<robot_interfaces::srv::ComputeArea::Request> request,
        std::shared_ptr<robot_interfaces::srv::ComputeArea::Response> response) {
        response->area = request->width * request->height;
        response->message = "ok";
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
  rclcpp::spin(std::make_shared<AreaServer>());
  rclcpp::shutdown();
  return 0;
}
