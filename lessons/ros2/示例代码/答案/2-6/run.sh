#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_answers a2_6 >/dev/null 2>&1 &
sleep 1
ros2 service call /compute_area robot_interfaces/srv/ComputeArea "{width: -2.0, height: 3.0}"
ros2 service call /compute_area robot_interfaces/srv/ComputeArea "{width: 0.0, height: 4.0}"
