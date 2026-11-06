#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 launch robot_bringup param_nodes.launch.xml >/tmp/tup_answer_5_6.log 2>&1 &
value=''
for _ in $(seq 1 20); do
  if value=$(ros2 param get /right/param_right rate_ms 2>/dev/null); then
    break
  fi
  sleep 0.5
done
printf '%s\n' "$value"
