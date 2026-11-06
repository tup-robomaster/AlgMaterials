#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

log=/tmp/tup_answer_6_3.log
ros2 run robot_nodes lifecycle_talker \
  --ros-args -p message:=calibration >"$log" 2>&1 &
wait_for service /lifecycle_talker/get_state

ros2 lifecycle set /lifecycle_talker configure >/dev/null
sleep 0.2
grep 'configured' "$log"
