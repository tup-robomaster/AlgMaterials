// 2-2 的答案：用自定义 Echo 服务把请求里的字符串写回响应。
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/srv/echo.hpp"

class EchoServer : public rclcpp::Node
{
public:
  EchoServer()
  : Node("echo_server")
  {
    service_ = create_service<robot_interfaces::srv::Echo>(
      "echo",
      [this](
        const std::shared_ptr<robot_interfaces::srv::Echo::Request> request,
        std::shared_ptr<robot_interfaces::srv::Echo::Response> response) {
        response->echo = request->text;
        RCLCPP_INFO(get_logger(), "request: '%s'", request->text.c_str());
        RCLCPP_INFO(get_logger(), "response: '%s'", response->echo.c_str());
      });
  }

private:
  rclcpp::Service<robot_interfaces::srv::Echo>::SharedPtr service_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<EchoServer>());
  rclcpp::shutdown();
  return 0;
}
