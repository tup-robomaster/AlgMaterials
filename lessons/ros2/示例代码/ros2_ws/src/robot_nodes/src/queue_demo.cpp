// 4.1 的回调队列示例：同一个单线程执行器里，发布者一次发出三条消息，
// 订阅者按到达顺序逐条处理，前一条处理完才轮到后一条。
#include <chrono>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"

using namespace std::chrono_literals;

class QueueTalker : public rclcpp::Node
{
public:
  explicit QueueTalker(const std::vector<int> & delays)
  : Node("queue_talker"), delays_(delays)
  {
    publisher_ = create_publisher<std_msgs::msg::Int32>("queue_delay", 10);
    timer_ = create_wall_timer(100ms, [this]() {
      if (publisher_->get_subscription_count() == 0) {
        return;
      }
      std_msgs::msg::Int32 message;
      for (int delay : delays_) {
        message.data = delay;
        publisher_->publish(message);
        RCLCPP_INFO(get_logger(), "sent %d", delay);
      }
      timer_->cancel();
    });
  }

private:
  std::vector<int> delays_;
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

class QueueListener : public rclcpp::Node
{
public:
  explicit QueueListener(std::size_t expected)
  : Node("queue_listener"), expected_(expected), count_(0)
  {
    subscription_ = create_subscription<std_msgs::msg::Int32>(
      "queue_delay", 10, [this](const std_msgs::msg::Int32::SharedPtr message) {
        const int delay = message->data;
        RCLCPP_INFO(get_logger(), "begin %d", delay);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        RCLCPP_INFO(get_logger(), "end %d", delay);
        ++count_;
        if (count_ == expected_) {
          RCLCPP_INFO(get_logger(), "all done");
          rclcpp::shutdown();
        }
      });
  }

private:
  std::size_t expected_;
  std::size_t count_;
  rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
  std::vector<int> delays;
  const std::string text = argc > 1 ? argv[1] : "400,0,0";
  std::stringstream stream(text);
  std::string item;
  while (std::getline(stream, item, ',')) {
    try {
      delays.push_back(std::stoi(item));
    } catch (const std::exception &) {
      return 2;
    }
  }
  if (delays.empty()) {
    return 2;
  }

  rclcpp::init(argc, argv);
  auto talker = std::make_shared<QueueTalker>(delays);
  auto listener = std::make_shared<QueueListener>(delays.size());
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(talker);
  executor.add_node(listener);
  executor.spin();
  rclcpp::shutdown();
  return 0;
}
