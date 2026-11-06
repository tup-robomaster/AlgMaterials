#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

broadcaster_log=$(mktemp)
listener_log=$(mktemp)
trap 'rm -f "$broadcaster_log" "$listener_log"' EXIT

setsid ros2 run part10_answers a10_6_broadcaster >"$broadcaster_log" 2>&1 &
broadcaster_pid=$!
wait_for topic /tf_static
wait_for topic /tf

setsid ros2 run part10_answers a10_6_listener >"$listener_log" 2>&1 &
listener_pid=$!
sleep 1.5

grep -E 'point in map:|lookup failed:' "$listener_log"

kill -TERM -- "-$listener_pid" "-$broadcaster_pid" 2>/dev/null || true
wait "$listener_pid" 2>/dev/null || true
wait "$broadcaster_pid" 2>/dev/null || true
sleep 1
