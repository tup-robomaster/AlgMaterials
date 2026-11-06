#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

server_log=$(mktemp)
publisher_log=$(mktemp)
call_log=$(mktemp)
trap 'rm -f "$server_log" "$publisher_log" "$call_log"' EXIT

setsid ros2 run part10_answers a10_3_server >"$server_log" 2>&1 &
server_pid=$!
wait_for service /scale

setsid ros2 run part10_answers a10_3_publisher >"$publisher_log" 2>&1 &
publisher_pid=$!
for _ in $(seq 1 40); do
  grep -q 'publish: right 3.5 #3' "$publisher_log" && break
  sleep 0.25
done
sleep 0.3

ros2 service call /scale answer_interfaces/srv/Scale "{factor: 2.0}" \
  >"$call_log" 2>&1
sleep 0.3

grep -E 'publish:' "$publisher_log"
grep -E 'latest:|scale:' "$server_log"

kill -TERM -- "-$publisher_pid" "-$server_pid" 2>/dev/null || true
wait "$publisher_pid" 2>/dev/null || true
wait "$server_pid" 2>/dev/null || true
sleep 1
