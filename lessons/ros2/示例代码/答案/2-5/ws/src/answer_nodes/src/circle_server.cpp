// 2-5 的答案：圆形面积服务端，按半径计算面积。
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/srv/circle_area.hpp"

class CircleServer : public rclcpp::Node
{
public:
  CircleServer()
  : Node("circle_server")
  {
    service_ = create_service<robot_interfaces::srv::CircleArea>(
      "circle_area",
      [this](
        const std::shared_ptr<robot_interfaces::srv::CircleArea::Request> request,
        std::shared_ptr<robot_interfaces::srv::CircleArea::Response> response) {
        response->area = 3.14159 * request->radius * request->radius;
        response->message = "ok";
        RCLCPP_INFO(get_logger(), "request: radius=%.2f", request->radius);
        RCLCPP_INFO(
          get_logger(), "response: area=%.2f message='%s'",
          response->area, response->message.c_str());
      });
  }

private:
  rclcpp::Service<robot_interfaces::srv::CircleArea>::SharedPtr service_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CircleServer>());
  rclcpp::shutdown();
  return 0;
}
