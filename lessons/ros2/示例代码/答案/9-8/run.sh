#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

echo "max_distance: 1.5"
bash "$ROS2_WS/src/robot_bringup/launch/9.4-verify.sh" 2 1.5 |
  grep "controller done:"
