#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

out=$(mktemp)
ros2 run robot_answers a1_8_listener > "$out" 2>&1 &
listener_pid=$!
sleep 1
ros2 run robot_answers a1_8_talker >/dev/null 2>&1
wait "$listener_pid"
cat "$out"
rm -f "$out"
