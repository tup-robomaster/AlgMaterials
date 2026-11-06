// 2-5 的答案客户端：向 circle_area 请求半径 2.0 的面积。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/srv/circle_area.hpp"

using namespace std::chrono_literals;

class CircleClient : public rclcpp::Node
{
public:
  CircleClient()
  : Node("circle_client")
  {
    client_ = create_client<robot_interfaces::srv::CircleArea>("circle_area");
  }

  bool send(double radius)
  {
    if (!client_->wait_for_service(1s)) {
      RCLCPP_ERROR(get_logger(), "service /circle_area not available after 1s");
      return false;
    }

    auto request = std::make_shared<robot_interfaces::srv::CircleArea::Request>();
    request->radius = radius;
    auto future = client_->async_send_request(request);
    if (rclcpp::spin_until_future_complete(shared_from_this(), future) !=
      rclcpp::FutureReturnCode::SUCCESS)
    {
      RCLCPP_ERROR(get_logger(), "request failed");
      return false;
    }

    auto response = future.get();
    RCLCPP_INFO(
      get_logger(), "area: %.2f m^2 message: %s",
      response->area, response->message.c_str());
    return true;
  }

private:
  rclcpp::Client<robot_interfaces::srv::CircleArea>::SharedPtr client_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CircleClient>();
  const bool ok = node->send(2.0);
  rclcpp::shutdown();
  return ok ? 0 : 1;
}
