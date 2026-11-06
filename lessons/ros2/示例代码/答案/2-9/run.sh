#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

params="$(dirname "$0")/two_nodes.yaml"
ros2 run robot_nodes parameter_node --ros-args --params-file "$params" >/dev/null 2>&1 &
ros2 run robot_nodes param_demo --ros-args --params-file "$params" >/dev/null 2>&1 &
wait_for node /parameter_node
wait_for node /param_demo
ros2 param get /parameter_node rate_ms
ros2 param get /parameter_node message
ros2 param get /param_demo rate_ms
ros2 param get /param_demo message
