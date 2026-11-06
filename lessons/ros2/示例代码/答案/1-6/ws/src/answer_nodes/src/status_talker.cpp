// 练习 1-6 的答案：发布自定义 Status 消息。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/msg/status.hpp"

using namespace std::chrono_literals;

class StatusTalker : public rclcpp::Node
{
public:
  StatusTalker()
  : Node("status_talker"), count_(0)
  {
    publisher_ = create_publisher<robot_interfaces::msg::Status>("status", 10);
    timer_ = create_wall_timer(500ms, [this]() {
      const char * states[3] = {"idle", "moving", "ready"};
      const int codes[3] = {0, 1, 2};

      robot_interfaces::msg::Status status;
      status.state = states[count_];
      status.code = codes[count_];

      publisher_->publish(status);
      RCLCPP_INFO(
        get_logger(), "state=%s code=%d",
        status.state.c_str(), status.code);
      ++count_;
      if (count_ == 3) {
        rclcpp::shutdown();
      }
    });
  }

private:
  rclcpp::Publisher<robot_interfaces::msg::Status>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StatusTalker>());
  rclcpp::shutdown();
  return 0;
}
