#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes dynamic_tf >/dev/null 2>&1 &
wait_for node /dynamic_tf
sleep 0.7

out=/tmp/tup_answer_7_6.log
timeout 3 ros2 topic echo --once /tf >"$out" 2>/dev/null || true
grep -E 'frame_id|child_frame_id' "$out" | sed 's/^ *//'
