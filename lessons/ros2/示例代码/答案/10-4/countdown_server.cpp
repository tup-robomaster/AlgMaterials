// 10-4 的动作服务端：每 200 毫秒发布一次剩余数，最后返回总数。
#include <chrono>
#include <memory>
#include <thread>

#include "answer_interfaces/action/countdown.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using namespace std::chrono_literals;

class CountdownServer : public rclcpp::Node
{
public:
  using Countdown = answer_interfaces::action::Countdown;
  using GoalHandle = rclcpp_action::ServerGoalHandle<Countdown>;

  CountdownServer()
  : Node("countdown_server")
  {
    server_ = rclcpp_action::create_server<Countdown>(
      this,
      "countdown",
      [this](
        const rclcpp_action::GoalUUID &,
        std::shared_ptr<const Countdown::Goal> goal) {
        if (goal->count <= 0) {
          RCLCPP_INFO(get_logger(), "goal rejected: count=%d", goal->count);
          return rclcpp_action::GoalResponse::REJECT;
        }
        RCLCPP_INFO(get_logger(), "goal accepted: count=%d", goal->count);
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
      },
      [](const std::shared_ptr<GoalHandle>) {
        return rclcpp_action::CancelResponse::ACCEPT;
      },
      [this](const std::shared_ptr<GoalHandle> goal_handle) {
        std::thread{&CountdownServer::execute, this, goal_handle}.detach();
      });
  }

private:
  void execute(const std::shared_ptr<GoalHandle> goal_handle)
  {
    const auto goal = goal_handle->get_goal();
    auto feedback = std::make_shared<Countdown::Feedback>();
    auto result = std::make_shared<Countdown::Result>();

    for (int32_t remaining = goal->count; remaining > 0; --remaining) {
      std::this_thread::sleep_for(200ms);
      feedback->remaining = remaining - 1;
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(
        get_logger(), "feedback: remaining=%d", feedback->remaining);
    }

    result->total = goal->count;
    goal_handle->succeed(result);
    RCLCPP_INFO(get_logger(), "result: total=%d", result->total);
  }

  rclcpp_action::Server<Countdown>::SharedPtr server_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CountdownServer>());
  rclcpp::shutdown();
  return 0;
}
