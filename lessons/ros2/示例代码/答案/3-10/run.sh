#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes patrol_server >/dev/null 2>&1 &
sleep 1
ros2 action send_goal /patrol robot_interfaces/action/Patrol "{steps: 4}" --feedback \
  | sed -E 's/[0-9a-f]{32}/<goal-id>/'
