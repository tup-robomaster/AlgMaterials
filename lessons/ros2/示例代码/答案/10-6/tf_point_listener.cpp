// 10-6 的监听者：把 sensor 坐标系里的点转换到 map，打印一次后关闭。
#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "geometry_msgs/msg/point_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

using namespace std::chrono_literals;

class TfPointListener : public rclcpp::Node
{
public:
  TfPointListener()
  : Node("tf_point_listener")
  {
    buffer_ = std::make_shared<tf2_ros::Buffer>(get_clock());
  }

  void start()
  {
    listener_ = std::make_shared<tf2_ros::TransformListener>(
      *buffer_, shared_from_this(), false);
    started_at_ = now();
    timer_ = create_wall_timer(100ms, [this]() { try_transform(); });
  }

private:
  void try_transform()
  {
    if (!buffer_->canTransform("map", "sensor", tf2::TimePointZero)) {
      if ((now() - started_at_).seconds() > 5.0) {
        std::cout << "lookup failed: map -> sensor not available\n";
        rclcpp::shutdown();
      }
      return;
    }

    const auto transform = buffer_->lookupTransform(
      "map", "sensor", tf2::TimePointZero);

    geometry_msgs::msg::PointStamped point;
    point.header.frame_id = "sensor";
    point.header.stamp = now();
    point.point.x = 0.2;

    geometry_msgs::msg::PointStamped converted;
    tf2::doTransform(point, converted, transform);
    std::cout << "point in map: (" << std::fixed << std::setprecision(3)
              << converted.point.x << ", " << converted.point.y << ", "
              << converted.point.z << ")\n";
    rclcpp::shutdown();
  }

  rclcpp::Time started_at_;
  std::shared_ptr<tf2_ros::Buffer> buffer_;
  std::shared_ptr<tf2_ros::TransformListener> listener_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TfPointListener>();
  node->start();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
