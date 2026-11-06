#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes lifecycle_talker \
  --ros-args -r __node:=alpha -p topic:=/alpha >/dev/null 2>&1 &
ros2 run robot_nodes lifecycle_talker \
  --ros-args -r __node:=beta -p topic:=/beta >/dev/null 2>&1 &
wait_for service /alpha/change_state
wait_for service /beta/change_state

ros2 run robot_nodes lifecycle_manager \
  --ros-args -p nodes:=/alpha,/beta -p hold_ms:=300 -p gap_ms:=200
