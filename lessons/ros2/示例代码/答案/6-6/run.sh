#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

faulty_log=/tmp/tup_answer_6_6.log
ros2 run robot_nodes lifecycle_talker \
  --ros-args -r __node:=alpha -p topic:=/alpha >/dev/null 2>&1 &
ros2 run robot_nodes lifecycle_talker \
  --ros-args -r __node:=faulty -p topic:=/faulty \
  -p fail_on_activate:=true >"$faulty_log" 2>&1 &
wait_for service /alpha/change_state
wait_for service /faulty/change_state

ros2 run robot_nodes lifecycle_manager \
  --ros-args -p nodes:=/alpha,/faulty -p hold_ms:=300 -p gap_ms:=200
grep 'activation refused' "$faulty_log"
