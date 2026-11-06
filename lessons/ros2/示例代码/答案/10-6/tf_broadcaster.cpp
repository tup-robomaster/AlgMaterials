// 10-6 的广播者：静态 map -> base_link，动态 base_link -> sensor。
#include <chrono>
#include <cmath>
#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/static_transform_broadcaster.h"
#include "tf2_ros/transform_broadcaster.h"

using namespace std::chrono_literals;

namespace
{
constexpr double kPi = 3.14159265358979323846;
}

class TfBroadcaster : public rclcpp::Node
{
public:
  TfBroadcaster()
  : Node("tf_broadcaster")
  {
    tf2::Quaternion yaw;
    yaw.setRPY(0.0, 0.0, 90.0 * kPi / 180.0);

    geometry_msgs::msg::TransformStamped map_to_base;
    map_to_base.header.stamp = now();
    map_to_base.header.frame_id = "map";
    map_to_base.child_frame_id = "base_link";
    map_to_base.transform.translation.x = 1.0;
    map_to_base.transform.translation.y = 2.0;
    map_to_base.transform.rotation.x = yaw.x();
    map_to_base.transform.rotation.y = yaw.y();
    map_to_base.transform.rotation.z = yaw.z();
    map_to_base.transform.rotation.w = yaw.w();

    static_broadcaster_ =
      std::make_shared<tf2_ros::StaticTransformBroadcaster>(*this);
    static_broadcaster_->sendTransform(map_to_base);
    RCLCPP_INFO(get_logger(), "static: map -> base_link");

    dynamic_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
    timer_ = create_wall_timer(100ms, [this]() { publish_sensor(); });
  }

private:
  void publish_sensor()
  {
    geometry_msgs::msg::TransformStamped base_to_sensor;
    base_to_sensor.header.stamp = now();
    base_to_sensor.header.frame_id = "base_link";
    base_to_sensor.child_frame_id = "sensor";
    base_to_sensor.transform.translation.x = 0.5;
    base_to_sensor.transform.rotation.w = 1.0;
    dynamic_broadcaster_->sendTransform(base_to_sensor);
  }

  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> static_broadcaster_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> dynamic_broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TfBroadcaster>());
  rclcpp::shutdown();
  return 0;
}
