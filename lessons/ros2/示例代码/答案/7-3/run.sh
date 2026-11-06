#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes static_tf --ros-args \
  -p child_frame:=camera_link -p x:=0.1 -p y:=0.0 -p z:=0.5 \
  -p yaw_deg:=0.0 >/dev/null 2>&1 &
wait_for node /static_tf
sleep 0.5

out=/tmp/tup_answer_7_3.log
timeout 2 ros2 run tf2_ros tf2_echo base_link camera_link >"$out" 2>/dev/null || true
grep -m1 -A3 -- '- Translation:' "$out"
