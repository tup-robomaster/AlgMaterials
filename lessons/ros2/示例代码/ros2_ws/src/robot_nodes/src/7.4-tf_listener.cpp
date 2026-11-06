// 7.4 的坐标监听：查询目标坐标系到源坐标系的变换，再把一个点变换过去。
// 找不到坐标系或变换时打印原因；目标、源与点坐标都可以用参数覆盖。
#include <chrono>
#include <cmath>
#include <iomanip>
#include <memory>
#include <string>

#include "geometry_msgs/msg/point_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

using namespace std::chrono_literals;

class TfListener : public rclcpp::Node
{
public:
  TfListener()
  : Node("tf_listener")
  {
    target_ = declare_parameter<std::string>("target_frame", "base_link");
    source_ = declare_parameter<std::string>("source_frame", "laser_link");
    px_ = declare_parameter<double>("px", 0.1);
    py_ = declare_parameter<double>("py", 0.2);
    pz_ = declare_parameter<double>("pz", 0.0);
    timeout_ms_ = declare_parameter<int>("timeout_ms", 3000);
    buffer_ = std::make_shared<tf2_ros::Buffer>(get_clock());
  }

  void start()
  {
    listener_ = std::make_shared<tf2_ros::TransformListener>(
      *buffer_, shared_from_this(), false);
    started_at_ = now();
    timer_ = create_wall_timer(200ms, [this]() { try_transform(); });
  }

private:
  void try_transform()
  {
    std::string error;
    if (buffer_->canTransform(target_, source_, tf2::TimePointZero, &error)) {
      try {
        const auto transform = buffer_->lookupTransform(
          target_, source_, tf2::TimePointZero);
        geometry_msgs::msg::PointStamped point;
        point.header.frame_id = source_;
        point.header.stamp = now();
        point.point.x = px_;
        point.point.y = py_;
        point.point.z = pz_;

        geometry_msgs::msg::PointStamped converted;
        tf2::doTransform(point, converted, transform);
        std::cout << "point (" << std::fixed << std::setprecision(3)
                  << px_ << ", " << py_ << ", " << pz_ << ") in " << target_
                  << ": (" << converted.point.x << ", " << converted.point.y
                  << ", " << converted.point.z << ")\n";
      } catch (const tf2::TransformException & exception) {
        std::cout << "lookup failed: " << exception.what() << "\n";
      }
      rclcpp::shutdown();
      return;
    }

    if ((now() - started_at_).seconds() * 1000.0 > timeout_ms_) {
      std::cout << "lookup failed: " << error << "\n";
      rclcpp::shutdown();
    }
  }

  std::string target_;
  std::string source_;
  double px_;
  double py_;
  double pz_;
  int timeout_ms_;
  rclcpp::Time started_at_;
  std::shared_ptr<tf2_ros::Buffer> buffer_;
  std::shared_ptr<tf2_ros::TransformListener> listener_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TfListener>();
  node->start();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
