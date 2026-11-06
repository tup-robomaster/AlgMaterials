#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

tmp=$(mktemp -d)
trap 'answer_cleanup; rm -rf "$tmp"' EXIT

ros2 run robot_nodes patrol_server >/dev/null 2>&1 &
server_pid=$!
ros2 run robot_nodes patrol_controller --ros-args -p initial_mode:=idle \
  >"$tmp/controller.log" 2>&1 &
controller_pid=$!
wait_for node /patrol_controller

ros2 run robot_nodes target_source --ros-args -p wait_for_subscriber:=true \
  >/dev/null 2>&1

for _ in $(seq 1 50); do
  if grep -q "target ignored: delta" "$tmp/controller.log"; then
    break
  fi
  sleep 0.1
done

ros2 service call /set_mode robot_interfaces/srv/SetMode "{mode: 'idle'}" \
  >"$tmp/service.log" 2>&1
sleep 0.3

pkill -TERM -P "$controller_pid" 2>/dev/null || true
kill -TERM "$controller_pid" 2>/dev/null || true
pkill -TERM -P "$server_pid" 2>/dev/null || true
kill -TERM "$server_pid" 2>/dev/null || true
wait "$controller_pid" 2>/dev/null || true
wait "$server_pid" 2>/dev/null || true

grep -E "target ignored|set_mode" "$tmp/controller.log"
sed -n '/response:/,$p' "$tmp/service.log"
