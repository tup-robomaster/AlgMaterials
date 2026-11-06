#include <chrono>
#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/msg/target.hpp"

using namespace std::chrono_literals;

namespace
{
struct TargetSpec
{
  double x;
  double y;
  const char * label;
};
}  // namespace

class TargetSource : public rclcpp::Node
{
public:
  TargetSource()
  : Node("target_source")
  {
    reverse_ = declare_parameter<bool>("reverse", false);
    max_distance_ = declare_parameter<double>("max_distance", 0.0);
    start_delay_ms_ = declare_parameter<int>("start_delay_ms", 0);
    wait_for_subscriber_ = declare_parameter<bool>("wait_for_subscriber", false);

    specs_ = {
      {1.0, 0.0, "alpha"},
      {2.0, 1.0, "beta"},
      {2.0, 1.0, "beta"},
      {0.5, -1.0, "gamma"},
      {3.0, 0.5, "delta"},
    };
    start_time_ = std::chrono::steady_clock::now();

    publisher_ = create_publisher<robot_interfaces::msg::Target>("target", 10);
    timer_ = create_wall_timer(400ms, [this]() {publish_next();});
  }

private:
  void publish_next()
  {
    const auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::steady_clock::now() - start_time_).count();
    if (elapsed < start_delay_ms_) {
      return;
    }
    if (wait_for_subscriber_ && index_ == 0 &&
      publisher_->get_subscription_count() == 0)
    {
      return;
    }
    if (index_ >= specs_.size()) {
      rclcpp::shutdown();
      return;
    }

    const TargetSpec & spec = specs_[reverse_ ? specs_.size() - 1 - index_ : index_];
    ++index_;

    const double distance = std::hypot(spec.x, spec.y);
    if (max_distance_ > 0.0 && distance > max_distance_) {
      RCLCPP_INFO(
        get_logger(), "skip: %s (%.2f, %.2f) distance=%.2f",
        spec.label, spec.x, spec.y, distance);
    } else {
      robot_interfaces::msg::Target target;
      target.x = spec.x;
      target.y = spec.y;
      target.label = spec.label;
      publisher_->publish(target);
      ++published_;
      RCLCPP_INFO(
        get_logger(), "publish: %s (%.2f, %.2f)", spec.label, spec.x, spec.y);
    }

    if (index_ >= specs_.size()) {
      RCLCPP_INFO(get_logger(), "done: published %zu targets", published_);
    }
  }

  bool reverse_{false};
  bool wait_for_subscriber_{false};
  double max_distance_{0.0};
  int start_delay_ms_{0};
  std::size_t index_{0};
  std::size_t published_{0};
  std::vector<TargetSpec> specs_;
  std::chrono::steady_clock::time_point start_time_;
  rclcpp::Publisher<robot_interfaces::msg::Target>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TargetSource>());
  rclcpp::shutdown();
  return 0;
}
