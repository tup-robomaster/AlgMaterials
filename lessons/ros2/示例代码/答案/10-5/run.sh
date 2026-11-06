#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

launch_log=$(mktemp)

cleanup() {
  kill -TERM -- "-$launch_pid" 2>/dev/null || true
  wait "$launch_pid" 2>/dev/null || true
}
trap 'cleanup; rm -f "$launch_log"' EXIT

setsid ros2 launch part10_answers 10.5-lab.launch.py >"$launch_log" 2>&1 &
launch_pid=$!
wait_for topic /lab/lab_chatter

ros2 topic list | grep '^/lab/' | sort
ros2 param get /lab/parameter_node message
ros2 param get /lab/parameter_node rate_ms
sleep 1.5

grep -E "publishing: 'hello" "$launch_log" | sed -E 's/^\[talker-[0-9]+\] //'
grep -E "I heard: 'hello" "$launch_log" | sed -E 's/^\[listener-[0-9]+\] //'

kill -TERM -- "-$launch_pid" 2>/dev/null || true
wait "$launch_pid" 2>/dev/null || true
sleep 1
