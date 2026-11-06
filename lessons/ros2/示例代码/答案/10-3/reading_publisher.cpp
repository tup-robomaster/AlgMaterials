// 10-3 的发布者：按固定序列发布三条读数，发完后关闭。
#include <chrono>
#include <memory>
#include <string>
#include <thread>

#include "answer_interfaces/msg/reading.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class ReadingPublisher : public rclcpp::Node
{
public:
  ReadingPublisher()
  : Node("reading_publisher"), index_(0)
  {
    const bool wait = declare_parameter<bool>("wait_for_subscriber", true);
    publisher_ = create_publisher<answer_interfaces::msg::Reading>("reading", 10);

    if (wait) {
      for (int i = 0; i < 60 && publisher_->get_subscription_count() == 0; ++i) {
        std::this_thread::sleep_for(50ms);
      }
    }

    timer_ = create_wall_timer(300ms, [this]() { publish_one(); });
  }

private:
  void publish_one()
  {
    static const char * sensors[] = {"left", "left", "right"};
    static const double values[] = {1.5, 2.0, 3.5};

    answer_interfaces::msg::Reading reading;
    reading.sensor = sensors[index_];
    reading.value = values[index_];
    reading.sequence = index_ + 1;
    publisher_->publish(reading);
    RCLCPP_INFO(
      get_logger(), "publish: %s %.1f #%d",
      reading.sensor.c_str(), reading.value, reading.sequence);

    ++index_;
    if (index_ == 3) {
      rclcpp::shutdown();
    }
  }

  int index_;
  rclcpp::Publisher<answer_interfaces::msg::Reading>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ReadingPublisher>());
  rclcpp::shutdown();
  return 0;
}
