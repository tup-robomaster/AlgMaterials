// 4.2 的执行器对照：两个定时器回调各睡 work_ms 毫秒，记录起止时刻，
// 最后看两段时间是否重叠。单线程执行器一定不重叠；多线程执行器在
// 两个互斥回调组之间可以重叠，放进同一个组又会串行。
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class OverlapProbe : public rclcpp::Node
{
public:
  OverlapProbe(bool separate_groups, int work_ms)
  : Node("executor_probe"), work_ms_(work_ms)
  {
    auto callback = [this](int index) {
      start_[index] = std::chrono::steady_clock::now();
      std::this_thread::sleep_for(std::chrono::milliseconds(work_ms_));
      end_[index] = std::chrono::steady_clock::now();
      if (++done_ == 2) {
        promise_.set_value();
      }
    };

    if (separate_groups) {
      group_a_ = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
      group_b_ = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
      timer_a_ = create_wall_timer(100ms, [callback]() {callback(0);}, group_a_);
      timer_b_ = create_wall_timer(100ms, [callback]() {callback(1);}, group_b_);
    } else {
      timer_a_ = create_wall_timer(100ms, [callback]() {callback(0);});
      timer_b_ = create_wall_timer(100ms, [callback]() {callback(1);});
    }
  }

  std::future<void> done()
  {
    return promise_.get_future();
  }

  bool overlap() const
  {
    return start_[1] < end_[0] && start_[0] < end_[1];
  }

private:
  int work_ms_;
  std::atomic<int> done_{0};
  std::promise<void> promise_;
  std::chrono::steady_clock::time_point start_[2];
  std::chrono::steady_clock::time_point end_[2];
  rclcpp::CallbackGroup::SharedPtr group_a_;
  rclcpp::CallbackGroup::SharedPtr group_b_;
  rclcpp::TimerBase::SharedPtr timer_a_;
  rclcpp::TimerBase::SharedPtr timer_b_;
};

int main(int argc, char ** argv)
{
  const std::string executor_kind = argc > 1 ? argv[1] : "single";
  const std::string group_kind = argc > 2 ? argv[2] : "separate";
  const int work_ms = argc > 3 ? std::stoi(argv[3]) : 200;
  if (executor_kind != "single" && executor_kind != "multi") {
    return 2;
  }
  if (group_kind != "separate" && group_kind != "same") {
    return 2;
  }

  rclcpp::init(argc, argv);
  auto node = std::make_shared<OverlapProbe>(group_kind == "separate", work_ms);
  auto finished = node->done();
  std::thread spinner;
  if (executor_kind == "multi") {
    spinner = std::thread([node]() {
      rclcpp::executors::MultiThreadedExecutor executor;
      executor.add_node(node);
      executor.spin();
    });
  } else {
    spinner = std::thread([node]() {
      rclcpp::executors::SingleThreadedExecutor executor;
      executor.add_node(node);
      executor.spin();
    });
  }

  if (finished.wait_for(3s) != std::future_status::ready) {
    rclcpp::shutdown();
    spinner.join();
    std::cout << executor_kind << " " << group_kind << ": timeout" << std::endl;
    return 1;
  }
  rclcpp::shutdown();
  spinner.join();

  std::cout << executor_kind;
  if (executor_kind == "multi") {
    std::cout << " " << group_kind;
  }
  if (work_ms != 200) {
    std::cout << " work_ms=" << work_ms;
  }
  std::cout << ": overlap=" << (node->overlap() ? "yes" : "no") << std::endl;
  return 0;
}
