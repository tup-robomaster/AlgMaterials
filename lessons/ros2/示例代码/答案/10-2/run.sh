#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

broken_log=$(mktemp)
fixed_log=$(mktemp)
trap 'rm -f "$broken_log" "$fixed_log"' EXIT

setsid ros2 launch robot_bringup 10.1-inspect.launch.py \
  start_server:=false >"$broken_log" 2>&1 &
broken_pid=$!
sleep 3

ros2 action info /patrol
ros2 action list -t | grep '^/.*patrol' | sort

for _ in $(seq 1 40); do
  grep -q 'action server /patrol not available' "$broken_log" && break
  sleep 0.25
done
grep 'action server /patrol not available' "$broken_log" |
  sed -E 's/^\[[a-z_]+-[0-9]+\] //'

kill -TERM -- "-$broken_pid" 2>/dev/null || true
wait "$broken_pid" 2>/dev/null || true
sleep 2

setsid ros2 launch robot_bringup 10.1-inspect.launch.py >"$fixed_log" 2>&1 &
fixed_pid=$!
for _ in $(seq 1 80); do
  grep -q 'controller done:' "$fixed_log" && break
  sleep 0.25
done
grep 'controller done:' "$fixed_log" |
  sed -E 's/^\[[a-z_]+-[0-9]+\] //'

kill -TERM -- "-$fixed_pid" 2>/dev/null || true
wait "$fixed_pid" 2>/dev/null || true
sleep 1
