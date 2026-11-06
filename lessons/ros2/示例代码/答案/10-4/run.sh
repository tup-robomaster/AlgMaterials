#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

server_log=$(mktemp)
client_log=$(mktemp)
trap 'rm -f "$server_log" "$client_log"' EXIT

setsid ros2 run part10_answers a10_4_server >"$server_log" 2>&1 &
server_pid=$!
wait_for action /countdown

setsid ros2 run part10_answers a10_4_client >"$client_log" 2>&1 &
client_pid=$!
sleep 2

grep -E 'goal accepted:|feedback:|result:' "$server_log"
grep -E 'goal accepted|feedback:|result:' "$client_log"

kill -TERM -- "-$client_pid" "-$server_pid" 2>/dev/null || true
wait "$client_pid" 2>/dev/null || true
wait "$server_pid" 2>/dev/null || true
sleep 1
