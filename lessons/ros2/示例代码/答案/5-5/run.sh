#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 launch robot_bringup param_nodes.launch.xml >/tmp/tup_answer_5_5.log 2>&1 &
for _ in $(seq 1 20); do
  count=$(ros2 node list 2>/dev/null | grep -cE '^/(left|right)/param_' || true)
  if [ "$count" -ge 2 ]; then
    break
  fi
  sleep 0.5
done
ros2 node list | grep -E '^/(left|right)/param_' | sort
