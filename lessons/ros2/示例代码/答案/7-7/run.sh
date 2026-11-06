#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes static_tf --ros-args \
  -p child_frame:=camera_link -p x:=0.1 -p y:=0.0 -p z:=0.5 \
  -p yaw_deg:=0.0 >/dev/null 2>&1 &
wait_for node /static_tf
sleep 0.3

ros2 run robot_nodes tf_listener --ros-args \
  -p source_frame:=camera_link -p px:=0.1 -p py:=0.2 -p pz:=0.0
