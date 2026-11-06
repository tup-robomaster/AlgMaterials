// 4.4 的耗时任务对照：阻塞执行时心跳定时器拿不到执行机会；
// 把工作放到独立线程后，执行器线程继续处理心跳回调。
#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class LongTaskDemo : public rclcpp::Node
{
public:
  explicit LongTaskDemo(bool threaded)
  : Node("long_task_demo"), threaded_(threaded)
  {
    heartbeat_ = create_wall_timer(50ms, [this]() {
      if (working_.load()) {
        ++heartbeats_;
      }
    });
    task_ = create_wall_timer(100ms, [this]() {
      task_->cancel();
      working_ = true;
      if (threaded_) {
        worker_ = std::thread([this]() {
          while (heartbeats_.load() == 0) {
            std::this_thread::sleep_for(1ms);
          }
          std::this_thread::sleep_for(250ms);
          heartbeats_during_ = heartbeats_.load();
          working_ = false;
          promise_.set_value();
        });
      } else {
        std::this_thread::sleep_for(300ms);
        heartbeats_during_ = heartbeats_.load();
        working_ = false;
        promise_.set_value();
      }
    });
  }

  ~LongTaskDemo() override
  {
    if (worker_.joinable()) {
      worker_.join();
    }
  }

  std::future<void> done()
  {
    return promise_.get_future();
  }

  int heartbeats_during() const
  {
    return heartbeats_during_;
  }

private:
  bool threaded_;
  std::atomic<bool> working_{false};
  std::atomic<int> heartbeats_{0};
  int heartbeats_during_{0};
  std::promise<void> promise_;
  std::thread worker_;
  rclcpp::TimerBase::SharedPtr heartbeat_;
  rclcpp::TimerBase::SharedPtr task_;
};

int main(int argc, char ** argv)
{
  const std::string mode = argc > 1 ? argv[1] : "blocking";
  if (mode != "blocking" && mode != "threaded") {
    return 2;
  }

  rclcpp::init(argc, argv);
  auto node = std::make_shared<LongTaskDemo>(mode == "threaded");
  auto finished = node->done();
  std::thread spinner([node]() {
    rclcpp::executors::SingleThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();
  });

  if (finished.wait_for(3s) != std::future_status::ready) {
    rclcpp::shutdown();
    spinner.join();
    std::cout << mode << ": timeout" << std::endl;
    return 1;
  }
  rclcpp::shutdown();
  spinner.join();

  const int count = node->heartbeats_during();
  if (mode == "threaded") {
    std::cout << "threaded: heartbeats while task="
              << (count > 0 ? "nonzero" : "0") << std::endl;
  } else {
    std::cout << "blocking: heartbeats while task=" << count << std::endl;
  }
  return 0;
}
