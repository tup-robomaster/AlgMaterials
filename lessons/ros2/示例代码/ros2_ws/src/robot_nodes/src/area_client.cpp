// 2.3 的客户端：依次请求 3 x 4 与 -2 x 3，打印面积与响应消息。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/srv/compute_area.hpp"

using namespace std::chrono_literals;

class AreaClient : public rclcpp::Node
{
public:
  AreaClient()
  : Node("area_client")
  {
    client_ = create_client<robot_interfaces::srv::ComputeArea>("compute_area");
  }

  bool call(double width, double height)
  {
    if (!client_->wait_for_service(1s)) {
      RCLCPP_ERROR(get_logger(), "service /compute_area not available after 1s");
      return false;
    }

    auto request = std::make_shared<robot_interfaces::srv::ComputeArea::Request>();
    request->width = width;
    request->height = height;
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
  rclcpp::Client<robot_interfaces::srv::ComputeArea>::SharedPtr client_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<AreaClient>();
  bool ok = node->call(3.0, 4.0);
  ok = node->call(-2.0, 3.0) && ok;
  rclcpp::shutdown();
  return ok ? 0 : 1;
}
