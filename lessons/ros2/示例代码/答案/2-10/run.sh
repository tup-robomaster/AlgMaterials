#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

params="$(ros2 pkg prefix robot_nodes)/share/robot_nodes/config/params.yaml"
ros2 run robot_nodes param_demo --ros-args \
  --params-file "$params" -p message:=command >/dev/null 2>&1 &
wait_for node /param_demo
ros2 param dump /param_demo
