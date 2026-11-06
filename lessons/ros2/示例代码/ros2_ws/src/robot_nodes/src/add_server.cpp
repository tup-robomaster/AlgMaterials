// 2.1 的服务端：用 AddTwoInts 提供 add_two_ints，返回两个 64 位整数的和。
#include <memory>

#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/rclcpp.hpp"

class AddServer : public rclcpp::Node
{
public:
  AddServer()
  : Node("add_server")
  {
    service_ = create_service<example_interfaces::srv::AddTwoInts>(
      "add_two_ints",
      [this](
        const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request,
        std::shared_ptr<example_interfaces::srv::AddTwoInts::Response> response) {
        response->sum = request->a + request->b;
        RCLCPP_INFO(get_logger(), "request: %ld + %ld", request->a, request->b);
        RCLCPP_INFO(get_logger(), "response: %ld", response->sum);
      });
  }

private:
  rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr service_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AddServer>());
  rclcpp::shutdown();
  return 0;
}
