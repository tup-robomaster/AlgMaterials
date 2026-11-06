#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

log=$(mktemp)
ros2 run robot_answers a2_8 >"$log" 2>&1 &
wait_for node /target_counter
ros2 param set /target_counter target_count 5
ros2 param set /target_counter target_count -2 || true
ros2 param get /target_counter target_count
sleep 0.3
cat "$log"
