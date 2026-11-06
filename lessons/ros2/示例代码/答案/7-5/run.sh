#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

log=/tmp/tup_answer_7_5.log
ros2 run robot_nodes dynamic_tf >"$log" 2>&1 &
for _ in $(seq 1 40); do
  grep -q 'step 4' "$log" && break
  sleep 0.25
done
grep 'step' "$log"
