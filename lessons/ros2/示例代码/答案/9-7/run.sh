#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

echo "steps_per_target: 1"
bash "$ROS2_WS/src/robot_bringup/launch/9.4-verify.sh" 1 |
  grep "controller done:"
