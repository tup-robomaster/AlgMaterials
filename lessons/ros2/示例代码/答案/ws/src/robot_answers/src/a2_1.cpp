// 2-1 的答案：复用 AddTwoInts 的两个整数字段，在 multiply 服务里返回乘积。
#include <memory>

#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/rclcpp.hpp"

class MultiplyServer : public rclcpp::Node
{
public:
  MultiplyServer()
  : Node("multiply_server")
  {
    service_ = create_service<example_interfaces::srv::AddTwoInts>(
      "multiply",
      [this](
        const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request,
        std::shared_ptr<example_interfaces::srv::AddTwoInts::Response> response) {
        response->sum = request->a * request->b;
        RCLCPP_INFO(get_logger(), "request: %ld x %ld", request->a, request->b);
        RCLCPP_INFO(get_logger(), "response: %ld", response->sum);
      });
  }

private:
  rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr service_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MultiplyServer>());
  rclcpp::shutdown();
  return 0;
}
