#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes static_tf >/dev/null 2>&1 &
wait_for node /static_tf
sleep 0.3

ros2 run robot_nodes tf_listener --ros-args -p source_frame:=map
