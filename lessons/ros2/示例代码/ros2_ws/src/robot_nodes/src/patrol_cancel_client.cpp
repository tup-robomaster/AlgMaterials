// 3.4 的取消客户端：发送 10 步目标，收到第二条反馈后请求取消。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_interfaces/action/patrol.hpp"

using namespace std::chrono_literals;

class PatrolCancelClient : public rclcpp::Node
{
public:
  using Patrol = robot_interfaces::action::Patrol;
  using GoalHandlePatrol = rclcpp_action::ClientGoalHandle<Patrol>;

  PatrolCancelClient()
  : Node("patrol_cancel_client"), feedback_count_(0)
  {
    client_ = rclcpp_action::create_client<Patrol>(this, "patrol");
  }

  bool send(int32_t steps)
  {
    if (!client_->wait_for_action_server(2s)) {
      RCLCPP_ERROR(get_logger(), "action server /patrol not available after 2s");
      return false;
    }

    auto goal = Patrol::Goal();
    goal.steps = steps;

    auto options = rclcpp_action::Client<Patrol>::SendGoalOptions();
    options.feedback_callback = [this](
      GoalHandlePatrol::SharedPtr goal_handle,
      const std::shared_ptr<const Patrol::Feedback> feedback) {
        RCLCPP_INFO(get_logger(), "feedback: remaining=%d", feedback->remaining);
        ++feedback_count_;
        if (feedback_count_ == 2) {
          RCLCPP_INFO(get_logger(), "cancel requested");
          client_->async_cancel_goal(goal_handle);
        }
      };
    options.result_callback = [this](const GoalHandlePatrol::WrappedResult & result) {
      if (result.code == rclcpp_action::ResultCode::CANCELED) {
        RCLCPP_INFO(get_logger(), "result: canceled");
      } else if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
        RCLCPP_INFO(get_logger(), "result: succeeded");
      } else {
        RCLCPP_INFO(get_logger(), "result: aborted");
      }
    };

    auto goal_future = client_->async_send_goal(goal, options);
    if (rclcpp::spin_until_future_complete(shared_from_this(), goal_future) !=
      rclcpp::FutureReturnCode::SUCCESS)
    {
      RCLCPP_ERROR(get_logger(), "send goal failed");
      return false;
    }
    auto goal_handle = goal_future.get();
    if (!goal_handle) {
      RCLCPP_ERROR(get_logger(), "goal rejected");
      return false;
    }
    RCLCPP_INFO(get_logger(), "goal accepted");

    auto result_future = client_->async_get_result(goal_handle);
    if (rclcpp::spin_until_future_complete(shared_from_this(), result_future) !=
      rclcpp::FutureReturnCode::SUCCESS)
    {
      RCLCPP_ERROR(get_logger(), "get result failed");
      return false;
    }
    return true;
  }

private:
  rclcpp_action::Client<Patrol>::SharedPtr client_;
  int feedback_count_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PatrolCancelClient>();
  const bool ok = node->send(10);
  rclcpp::shutdown();
  return ok ? 0 : 1;
}
