#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

tmp=$(mktemp -d)
bag="$tmp/bag"
trap 'answer_cleanup; rm -rf "$tmp"' EXIT

timeout -s INT 6s ros2 bag record -o "$bag" --include-unpublished-topics /chatter \
  >"$tmp/record.log" 2>&1 &
record_pid=$!
sleep 1

ros2 run robot_nodes talker 3 --ros-args -p wait_for_subscriber:=true
wait "$record_pid" || true

timeout 15 ros2 topic echo /chatter std_msgs/msg/String --once --field data >"$tmp/echo.txt" 2>&1 &
echo_pid=$!
sleep 1
ros2 bag play --delay 2 --rate 0.2 "$bag" >/dev/null 2>&1
wait "$echo_pid"
cat "$tmp/echo.txt"
