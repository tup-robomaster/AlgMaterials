#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes static_tf >/dev/null 2>&1 &
wait_for node /static_tf
sleep 0.5

out=/tmp/tup_answer_7_4.log
timeout 2 ros2 run tf2_ros tf2_echo base_link laser_link >"$out" 2>/dev/null || true
grep -m1 -- '- Translation:' "$out"
