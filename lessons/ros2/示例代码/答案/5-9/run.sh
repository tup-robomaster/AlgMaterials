#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

container="$(ros2 pkg prefix rclcpp_components)/lib/rclcpp_components/component_container"
"$container" >/tmp/tup_answer_5_9.log 2>&1 &
wait_for node /ComponentManager
ros2 component load /ComponentManager robot_nodes robot_nodes::TalkerComponent >/dev/null
ros2 component list
