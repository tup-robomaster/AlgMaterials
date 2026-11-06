#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class LoggingDemo : public rclcpp::Node
{
public:
  LoggingDemo()
  : Node("logging_demo")
  {
    burst_ = declare_parameter<bool>("burst_throttled", false);

    RCLCPP_DEBUG(get_logger(), "debug: 这条默认不显示");
    RCLCPP_INFO(get_logger(), "info: 节点启动");
    RCLCPP_WARN(get_logger(), "warn: 电池电量 35%%");
    RCLCPP_ERROR(get_logger(), "error: 激光雷达未就绪");

    if (burst_) {
      RCLCPP_INFO(get_logger(), "burst: 连续调用 5 次节流日志");
      for (int i = 0; i < 5; ++i) {
        RCLCPP_INFO_THROTTLE(
          get_logger(), *get_clock(), 1000, "throttled: 同一秒内只显示一次");
      }
      RCLCPP_INFO(get_logger(), "burst: 调用结束");
      timer_ = create_wall_timer(
        200ms, [this]() {
          RCLCPP_INFO(get_logger(), "done");
          rclcpp::shutdown();
        });
      return;
    }

    timer_ = create_wall_timer(100ms, [this]() {tick();});
  }

private:
  void tick()
  {
    ++tick_;
    RCLCPP_DEBUG(get_logger(), "debug: tick=%d", tick_);
    RCLCPP_INFO(get_logger(), "tick=%d", tick_);
    if (tick_ == 3) {
      RCLCPP_WARN(get_logger(), "warn: tick=3 温度 41.0");
    }
    if (tick_ == 6) {
      RCLCPP_ERROR(get_logger(), "error: tick=6 雷达丢帧");
    }
    if (tick_ >= 8) {
      RCLCPP_INFO(get_logger(), "done");
      rclcpp::shutdown();
    }
  }

  bool burst_{false};
  int tick_{0};
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LoggingDemo>());
  rclcpp::shutdown();
  return 0;
}
