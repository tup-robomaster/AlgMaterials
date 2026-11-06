// 2-3 的答案：请求 add_two_ints 计算 10 + 20，打印请求与响应。
#include <chrono>
#include <memory>

#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class SumClient : public rclcpp::Node
{
public:
  SumClient()
  : Node("sum_client")
  {
    client_ = create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");
  }

  bool call(int64_t a, int64_t b)
  {
    if (!client_->wait_for_service(1s)) {
      RCLCPP_ERROR(get_logger(), "service /add_two_ints not available after 1s");
      return false;
    }

    auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
    request->a = a;
    request->b = b;
    auto future = client_->async_send_request(request);
    if (rclcpp::spin_until_future_complete(shared_from_this(), future) !=
      rclcpp::FutureReturnCode::SUCCESS)
    {
      RCLCPP_ERROR(get_logger(), "request failed");
      return false;
    }

    auto response = future.get();
    RCLCPP_INFO(get_logger(), "request: %ld + %ld", a, b);
    RCLCPP_INFO(get_logger(), "response: %ld", response->sum);
    return true;
  }

private:
  rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SumClient>();
  const bool ok = node->call(10, 20);
  rclcpp::shutdown();
  return ok ? 0 : 1;
}
