#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

work=$(mktemp -d)
recorder_log="$work/recorder.log"
echo_log="$work/echo.log"
trap 'rm -rf "$work"' EXIT

bag="$work/lab_bag"
setsid ros2 bag record --topics /target -o "$bag" >"$recorder_log" 2>&1 &
recorder_pid=$!
sleep 1

setsid ros2 run robot_nodes target_source --ros-args \
  -p wait_for_subscriber:=true >/dev/null 2>&1
sleep 1

kill -TERM -- "-$recorder_pid" 2>/dev/null || true
for _ in $(seq 1 20); do
  [[ -f "$bag/metadata.yaml" ]] && break
  sleep 0.25
done
sleep 0.5

ros2 bag info "$bag" | grep -E '^Messages:|^Topic information:'

setsid ros2 topic echo /target --once >"$echo_log" 2>&1 &
echo_pid=$!
setsid ros2 bag play --delay 2.0 "$bag" >/dev/null 2>&1 &
play_pid=$!
for _ in $(seq 1 40); do
  kill -0 "$echo_pid" 2>/dev/null || break
  sleep 0.25
done
kill -TERM -- "-$play_pid" 2>/dev/null || true
wait "$play_pid" 2>/dev/null || true
cat "$echo_log"
kill -TERM -- "-$echo_pid" 2>/dev/null || true
wait "$echo_pid" 2>/dev/null || true
sleep 1
