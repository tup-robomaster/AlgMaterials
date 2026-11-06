// 6.3 的生命周期管理节点：按参数给出的顺序配置、激活，再按相反顺序停用、清理。
// 某一步失败时立即停下，不再执行后面的状态转换。
#include <chrono>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/srv/change_state.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;
using Transition = lifecycle_msgs::msg::Transition;

class LifecycleManager : public rclcpp::Node
{
public:
  LifecycleManager()
  : Node("lifecycle_manager")
  {
    nodes_ = split(declare_parameter<std::string>("nodes", ""));
    hold_ms_ = declare_parameter<int>("hold_ms", 1200);
    gap_ms_ = declare_parameter<int>("gap_ms", 300);
    for (const auto & node : nodes_) {
      clients_[node] = create_client<lifecycle_msgs::srv::ChangeState>(
        node + "/change_state");
    }
  }

  bool run()
  {
    if (nodes_.empty()) {
      RCLCPP_ERROR(get_logger(), "parameter 'nodes' is empty");
      return false;
    }

    for (const auto & node : nodes_) {
      if (!clients_[node]->wait_for_service(3s)) {
        RCLCPP_ERROR(get_logger(), "%s/change_state is not available", node.c_str());
        return false;
      }
    }

    for (const auto & node : nodes_) {
      if (!change(node, Transition::TRANSITION_CONFIGURE, "configure")) {
        RCLCPP_ERROR(get_logger(), "stopped after failure");
        return false;
      }
    }
    for (const auto & node : nodes_) {
      if (!change(node, Transition::TRANSITION_ACTIVATE, "activate")) {
        RCLCPP_ERROR(get_logger(), "stopped after failure");
        return false;
      }
      rclcpp::sleep_for(std::chrono::milliseconds(gap_ms_));
    }

    rclcpp::sleep_for(std::chrono::milliseconds(hold_ms_));

    for (auto it = nodes_.rbegin(); it != nodes_.rend(); ++it) {
      if (!change(*it, Transition::TRANSITION_DEACTIVATE, "deactivate")) {
        RCLCPP_ERROR(get_logger(), "stopped after failure");
        return false;
      }
    }
    for (auto it = nodes_.rbegin(); it != nodes_.rend(); ++it) {
      if (!change(*it, Transition::TRANSITION_CLEANUP, "cleanup")) {
        RCLCPP_ERROR(get_logger(), "stopped after failure");
        return false;
      }
    }

    RCLCPP_INFO(get_logger(), "done");
    return true;
  }

private:
  static std::vector<std::string> split(const std::string & text)
  {
    std::vector<std::string> out;
    std::istringstream stream(text);
    std::string item;
    while (std::getline(stream, item, ',')) {
      if (!item.empty()) {
        out.push_back(item);
      }
    }
    return out;
  }

  bool change(const std::string & node, uint8_t id, const char * action)
  {
    auto request = std::make_shared<lifecycle_msgs::srv::ChangeState::Request>();
    request->transition.id = id;
    auto future = clients_[node]->async_send_request(request);
    const auto code = rclcpp::spin_until_future_complete(
      shared_from_this(), future, 2s);
    if (code != rclcpp::FutureReturnCode::SUCCESS) {
      RCLCPP_ERROR(get_logger(), "%s %s: service call failed", action, node.c_str());
      return false;
    }
    if (!future.get()->success) {
      RCLCPP_ERROR(get_logger(), "%s %s: failed", action, node.c_str());
      return false;
    }
    RCLCPP_INFO(get_logger(), "%s %s: success", action, node.c_str());
    return true;
  }

  std::vector<std::string> nodes_;
  int hold_ms_;
  int gap_ms_;
  std::map<std::string, rclcpp::Client<lifecycle_msgs::srv::ChangeState>::SharedPtr>
  clients_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto manager = std::make_shared<LifecycleManager>();
  manager->run();
  rclcpp::shutdown();
  return 0;
}
