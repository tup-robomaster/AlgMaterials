#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

tmp=$(mktemp -d)
trap 'answer_cleanup; rm -rf "$tmp"' EXIT

timeout 15 ros2 topic echo /chatter std_msgs/msg/String --once --field data >"$tmp/echo.txt" 2>&1 &
echo_pid=$!
sleep 1

ros2 run robot_nodes talker 1 --ros-args -p wait_for_subscriber:=true
wait "$echo_pid"
cat "$tmp/echo.txt"
