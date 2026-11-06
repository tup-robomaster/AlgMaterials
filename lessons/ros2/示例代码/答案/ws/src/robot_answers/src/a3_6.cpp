// 3-6 的答案：打印目标是否被接受与最终状态。
#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_interfaces/action/patrol.hpp"

using namespace std::chrono_literals;

const char * status_name(rclcpp_action::ResultCode code)
{
  switch (code) {
    case rclcpp_action::ResultCode::SUCCEEDED:
      return "SUCCEEDED";
    case rclcpp_action::ResultCode::CANCELED:
      return "CANCELED";
    case rclcpp_action::ResultCode::ABORTED:
      return "ABORTED";
    default:
      return "UNKNOWN";
  }
}

class StatusClient : public rclcpp::Node
{
public:
  using Patrol = robot_interfaces::action::Patrol;
  using GoalHandlePatrol = rclcpp_action::ClientGoalHandle<Patrol>;

  StatusClient()
  : Node("status_client")
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
    options.result_callback = [this](const GoalHandlePatrol::WrappedResult & result) {
      RCLCPP_INFO(get_logger(), "final status: %s", status_name(result.code));
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
      RCLCPP_INFO(get_logger(), "goal accepted: false");
      return false;
    }
    RCLCPP_INFO(get_logger(), "goal accepted: true");

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
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<StatusClient>();
  const bool ok = node->send(4);
  rclcpp::shutdown();
  return ok ? 0 : 1;
}
