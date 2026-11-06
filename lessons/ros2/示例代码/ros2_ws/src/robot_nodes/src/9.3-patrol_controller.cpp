#include <chrono>
#include <deque>
#include <memory>
#include <set>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_interfaces/action/patrol.hpp"
#include "robot_interfaces/msg/patrol_status.hpp"
#include "robot_interfaces/msg/target.hpp"
#include "robot_interfaces/srv/set_mode.hpp"

using namespace std::chrono_literals;
using Patrol = robot_interfaces::action::Patrol;
using GoalHandle = rclcpp_action::ClientGoalHandle<Patrol>;

class PatrolController : public rclcpp::Node
{
public:
  PatrolController()
  : Node("patrol_controller")
  {
    initial_mode_ = declare_parameter<std::string>("initial_mode", "patrol");
    steps_per_target_ = declare_parameter<int>("steps_per_target", 2);
    dedupe_ = declare_parameter<bool>("dedupe", true);
    finish_when_empty_ = declare_parameter<bool>("finish_when_empty", true);
    idle_timeout_sec_ = declare_parameter<double>("idle_timeout", 1.0);

    mode_ = valid_mode(initial_mode_) ? initial_mode_ : "idle";
    if (mode_ != initial_mode_) {
      RCLCPP_WARN(
        get_logger(), "unknown initial_mode '%s', use idle", initial_mode_.c_str());
    }

    target_sub_ = create_subscription<robot_interfaces::msg::Target>(
      "target", 10, [this](const robot_interfaces::msg::Target::SharedPtr msg) {
        on_target(*msg);
      });

    status_pub_ = create_publisher<robot_interfaces::msg::PatrolStatus>(
      "patrol_status", 10);

    mode_srv_ = create_service<robot_interfaces::srv::SetMode>(
      "set_mode",
      [this](
        const std::shared_ptr<robot_interfaces::srv::SetMode::Request> request,
        std::shared_ptr<robot_interfaces::srv::SetMode::Response> response) {
        set_mode(*request, *response);
      });

    action_client_ = rclcpp_action::create_client<Patrol>(this, "patrol");
    if (!action_client_->wait_for_action_server(5s)) {
      RCLCPP_ERROR(get_logger(), "action server /patrol not available");
      rclcpp::shutdown();
      return;
    }

    last_target_time_ = std::chrono::steady_clock::now();
    timer_ = create_wall_timer(100ms, [this]() {tick();});
    publish_status();
  }

private:
  static bool valid_mode(const std::string & mode)
  {
    return mode == "idle" || mode == "patrol" || mode == "stop";
  }

  void on_target(const robot_interfaces::msg::Target & target)
  {
    ++received_;
    last_target_time_ = std::chrono::steady_clock::now();

    if (mode_ != "patrol") {
      RCLCPP_INFO(
        get_logger(), "target ignored: %s mode=%s",
        target.label.c_str(), mode_.c_str());
      publish_status();
      return;
    }

    const std::string key = target.label + "|" + std::to_string(target.x) + "|" +
      std::to_string(target.y);
    if (dedupe_ && seen_.count(key) > 0) {
      ++duplicates_;
      RCLCPP_INFO(get_logger(), "duplicate ignored: %s", target.label.c_str());
      publish_status();
      return;
    }
    seen_.insert(key);
    queue_.push_back(target);
    ++accepted_;
    RCLCPP_INFO(
      get_logger(), "target queued: %s (%.2f, %.2f)",
      target.label.c_str(), target.x, target.y);
    publish_status();
  }

  void set_mode(
    const robot_interfaces::srv::SetMode::Request & request,
    robot_interfaces::srv::SetMode::Response & response)
  {
    if (!valid_mode(request.mode)) {
      response.success = false;
      response.message = "unknown mode: " + request.mode;
      RCLCPP_WARN(get_logger(), "set_mode failed: %s", response.message.c_str());
      return;
    }

    mode_ = request.mode;
    response.success = true;
    response.message = "mode=" + mode_;
    RCLCPP_INFO(get_logger(), "set_mode: %s success=true", mode_.c_str());

    if (mode_ == "stop") {
      queue_.clear();
      if (active_goal_) {
        RCLCPP_INFO(get_logger(), "cancel requested: %s", current_target_.c_str());
        action_client_->async_cancel_goal(active_goal_);
      }
    }
    publish_status();
  }

  void start_next()
  {
    current_target_msg_ = queue_.front();
    queue_.pop_front();
    current_target_ = current_target_msg_.label;
    current_x_ = current_target_msg_.x;
    current_y_ = current_target_msg_.y;
    active_ = true;
    publish_status();

    Patrol::Goal goal;
    goal.steps = steps_per_target_;

    auto options = rclcpp_action::Client<Patrol>::SendGoalOptions();
    options.goal_response_callback =
      [this](GoalHandle::SharedPtr handle) {
        if (!handle) {
          ++failed_;
          active_ = false;
          active_goal_.reset();
          RCLCPP_ERROR(
            get_logger(), "goal rejected: %s", current_target_.c_str());
          publish_status();
          return;
        }
        active_goal_ = handle;
        RCLCPP_INFO(
          get_logger(), "goal accepted: %s steps=%d",
          current_target_.c_str(), steps_per_target_);
      };
    options.feedback_callback =
      [this](GoalHandle::SharedPtr, const std::shared_ptr<const Patrol::Feedback> feedback) {
        RCLCPP_INFO(
          get_logger(), "feedback: %s remaining=%d",
          current_target_.c_str(), feedback->remaining);
      };
    options.result_callback =
      [this](const GoalHandle::WrappedResult & result) {
        active_ = false;
        active_goal_.reset();
        if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
          ++completed_;
          RCLCPP_INFO(
            get_logger(), "result: %s success=true", current_target_.c_str());
        } else if (result.code == rclcpp_action::ResultCode::CANCELED) {
          ++canceled_;
          RCLCPP_INFO(
            get_logger(), "result: %s canceled=true", current_target_.c_str());
        } else {
          ++failed_;
          RCLCPP_INFO(
            get_logger(), "result: %s success=false", current_target_.c_str());
        }
        publish_status();
      };

    action_client_->async_send_goal(goal, options);
  }

  void tick()
  {
    publish_status();

    if (mode_ == "stop" && !active_) {
      finish();
      return;
    }
    if (mode_ != "patrol") {
      return;
    }
    if (!active_ && !queue_.empty()) {
      start_next();
      return;
    }
    if (finish_when_empty_ && !active_ && queue_.empty() && received_ > 0) {
      const double idle =
        std::chrono::duration<double>(
        std::chrono::steady_clock::now() - last_target_time_).count();
      if (idle >= idle_timeout_sec_) {
        finish();
      }
    }
  }

  void finish()
  {
    if (finished_) {
      return;
    }
    finished_ = true;
    RCLCPP_INFO(
      get_logger(),
      "controller done: received=%d accepted=%zu completed=%d failed=%d canceled=%d",
      received_, accepted_, completed_, failed_, canceled_);
    publish_status();
    rclcpp::shutdown();
  }

  void publish_status()
  {
    robot_interfaces::msg::PatrolStatus status;
    status.state = mode_;
    status.current_target = current_target_.empty() ? "-" : current_target_;
    status.x = current_x_;
    status.y = current_y_;
    status.completed = completed_;
    status.failed = failed_;
    status.canceled = canceled_;
    status_pub_->publish(status);
  }

  std::string initial_mode_;
  std::string mode_;
  std::string current_target_;
  robot_interfaces::msg::Target current_target_msg_;
  double current_x_{0.0};
  double current_y_{0.0};
  int steps_per_target_{2};
  bool dedupe_{true};
  bool finish_when_empty_{true};
  bool active_{false};
  bool finished_{false};
  double idle_timeout_sec_{1.0};
  int received_{0};
  int duplicates_{0};
  int completed_{0};
  int failed_{0};
  int canceled_{0};
  std::size_t accepted_{0};
  std::set<std::string> seen_;
  std::deque<robot_interfaces::msg::Target> queue_;
  std::chrono::steady_clock::time_point last_target_time_;
  GoalHandle::SharedPtr active_goal_;
  rclcpp::Subscription<robot_interfaces::msg::Target>::SharedPtr target_sub_;
  rclcpp::Publisher<robot_interfaces::msg::PatrolStatus>::SharedPtr status_pub_;
  rclcpp::Service<robot_interfaces::srv::SetMode>::SharedPtr mode_srv_;
  rclcpp_action::Client<Patrol>::SharedPtr action_client_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PatrolController>();
  // 等待服务端失败时构造函数里已经 shutdown，这时不再进入 spin。
  if (rclcpp::ok()) {
    rclcpp::spin(node);
  }
  rclcpp::shutdown();
  return 0;
}
