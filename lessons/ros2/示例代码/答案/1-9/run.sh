#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes multi_talker left 10 --ros-args -r __node:=talker_a >/dev/null 2>&1 &
ros2 run robot_nodes multi_talker right 10 --ros-args -r __node:=talker_b >/dev/null 2>&1 &
sleep 1.5

for _ in $(seq 1 40); do
  info=$(ros2 topic info /chatter)
  if grep -q 'Publisher count: 2' <<<"$info"; then
    break
  fi
  sleep 0.25
done
grep -E 'Publisher count|Subscription count' <<<"$info"
wait
