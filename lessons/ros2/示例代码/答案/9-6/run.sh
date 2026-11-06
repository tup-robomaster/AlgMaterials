#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

tmp=$(mktemp -d)
trap 'answer_cleanup; rm -rf "$tmp"' EXIT

ros2 run robot_nodes patrol_server >/dev/null 2>&1 &
server_pid=$!
ros2 run robot_nodes patrol_controller --ros-args -p steps_per_target:=0 \
  >"$tmp/controller.log" 2>&1 &
controller_pid=$!
wait_for node /patrol_controller

ros2 run robot_nodes target_source --ros-args -p wait_for_subscriber:=true \
  >/dev/null 2>&1

wait "$controller_pid"
pkill -TERM -P "$server_pid" 2>/dev/null || true
kill -TERM "$server_pid" 2>/dev/null || true
wait "$server_pid" 2>/dev/null || true
grep -E "goal rejected|controller done" "$tmp/controller.log"
