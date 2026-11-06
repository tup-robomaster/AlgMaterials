// 3-3 的答案：动作服务端把反馈改成剩余步数的两倍。
#include <atomic>
#include <chrono>
#include <memory>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_interfaces/action/patrol.hpp"

using namespace std::chrono_literals;

class DoubleFeedbackServer : public rclcpp::Node
{
public:
  using Patrol = robot_interfaces::action::Patrol;
  using GoalHandlePatrol = rclcpp_action::ServerGoalHandle<Patrol>;

  DoubleFeedbackServer()
  : Node("double_feedback_server"), busy_(false)
  {
    server_ = rclcpp_action::create_server<Patrol>(
      this,
      "patrol",
      [this](
        const rclcpp_action::GoalUUID & uuid,
        std::shared_ptr<const Patrol::Goal> goal) {
        return handle_goal(uuid, goal);
      },
      [this](const std::shared_ptr<GoalHandlePatrol> goal_handle) {
        return handle_cancel(goal_handle);
      },
      [this](const std::shared_ptr<GoalHandlePatrol> goal_handle) {
        handle_accepted(goal_handle);
      });
  }

private:
  rclcpp_action::GoalResponse handle_goal(
    const rclcpp_action::GoalUUID &,
    std::shared_ptr<const Patrol::Goal> goal)
  {
    if (goal->steps <= 0 || busy_.load()) {
      RCLCPP_INFO(get_logger(), "goal rejected: steps=%d", goal->steps);
      return rclcpp_action::GoalResponse::REJECT;
    }
    RCLCPP_INFO(get_logger(), "goal accepted: steps=%d", goal->steps);
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse handle_cancel(
    const std::shared_ptr<GoalHandlePatrol> goal_handle)
  {
    RCLCPP_INFO(
      get_logger(), "cancel requested: steps=%d",
      goal_handle->get_goal()->steps);
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void handle_accepted(const std::shared_ptr<GoalHandlePatrol> goal_handle)
  {
    busy_.store(true);
    std::thread{&DoubleFeedbackServer::execute, this, goal_handle}.detach();
  }

  void execute(const std::shared_ptr<GoalHandlePatrol> goal_handle)
  {
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<Patrol::Feedback>();
    auto result = std::make_shared<Patrol::Result>();

    for (int32_t remaining = goal->steps - 1; remaining >= 0; --remaining) {
      std::this_thread::sleep_for(1s);
      if (goal_handle->is_canceling()) {
        result->success = false;
        busy_.store(false);
        goal_handle->canceled(result);
        RCLCPP_INFO(get_logger(), "goal canceled");
        return;
      }
      feedback->remaining = remaining * 2;
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(get_logger(), "feedback: remaining=%d", feedback->remaining);
    }

    result->success = true;
    busy_.store(false);
    goal_handle->succeed(result);
    RCLCPP_INFO(get_logger(), "goal succeeded");
  }

  rclcpp_action::Server<Patrol>::SharedPtr server_;
  std::atomic<bool> busy_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DoubleFeedbackServer>());
  rclcpp::shutdown();
  return 0;
}
