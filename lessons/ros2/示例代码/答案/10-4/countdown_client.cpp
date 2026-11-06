// 10-4 的动作客户端：发送 count=3 的目标，打印反馈与结果。
#include <chrono>
#include <memory>

#include "answer_interfaces/action/countdown.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using namespace std::chrono_literals;

class CountdownClient : public rclcpp::Node
{
public:
  using Countdown = answer_interfaces::action::Countdown;

  CountdownClient()
  : Node("countdown_client")
  {
    client_ = rclcpp_action::create_client<Countdown>(this, "countdown");
    if (!client_->wait_for_action_server(5s)) {
      RCLCPP_ERROR(get_logger(), "action server /countdown not available");
      rclcpp::shutdown();
    }
  }

  bool ready() const { return rclcpp::ok(); }

  void send()
  {
    Countdown::Goal goal;
    goal.count = 3;

    auto options = rclcpp_action::Client<Countdown>::SendGoalOptions();
    options.goal_response_callback =
      [this](const GoalHandle::SharedPtr & goal_handle) {
        if (!goal_handle) {
          RCLCPP_INFO(get_logger(), "goal rejected");
          return;
        }
        RCLCPP_INFO(get_logger(), "goal accepted");
      };
    options.feedback_callback =
      [this](
        GoalHandle::SharedPtr,
        const std::shared_ptr<const Countdown::Feedback> feedback) {
        RCLCPP_INFO(
          get_logger(), "feedback: remaining=%d", feedback->remaining);
      };
    options.result_callback =
      [this](const GoalHandle::WrappedResult & result) {
        const bool success =
          result.code == rclcpp_action::ResultCode::SUCCEEDED;
        RCLCPP_INFO(
          get_logger(), "result: total=%d success=%s",
          result.result->total, success ? "true" : "false");
        rclcpp::shutdown();
      };
    client_->async_send_goal(goal, options);
  }

private:
  using GoalHandle = rclcpp_action::ClientGoalHandle<Countdown>;
  rclcpp_action::Client<Countdown>::SharedPtr client_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<CountdownClient>();
  if (node->ready()) {
    node->send();
    rclcpp::spin(node);
  }
  rclcpp::shutdown();
  return 0;
}
