// 2-4 的答案：等待不存在的服务 1 秒，打印错误后正常退出。
#include <chrono>
#include <memory>

#include "example_interfaces/srv/add_two_ints.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class MissingClient : public rclcpp::Node
{
public:
  MissingClient()
  : Node("missing_client")
  {
    client_ = create_client<example_interfaces::srv::AddTwoInts>("missing_service");
  }

  bool wait()
  {
    return client_->wait_for_service(1s);
  }

private:
  rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<MissingClient>();
  if (!node->wait()) {
    RCLCPP_ERROR(
      node->get_logger(), "service /missing_service not available after 1s");
  }
  rclcpp::shutdown();
  return 0;
}
