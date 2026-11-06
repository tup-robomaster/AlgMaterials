#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

launch_log=$(mktemp)
trap 'rm -f "$launch_log"' EXIT

setsid ros2 launch robot_bringup 10.1-inspect.launch.py \
  start_delay_ms:=6000 >"$launch_log" 2>&1 &
launch_pid=$!
sleep 3

for node in $(ros2 node list | sort); do
  echo "$node"
  ros2 node info "$node" | awk '
    /Publishers:/ {sec = "pub"; next}
    /Subscribers:/ {sec = "sub"; next}
    /Service Servers:|Service Clients:|Action Servers:|Action Clients:/ {
      sec = ""
      next
    }
    /^[[:space:]]+\// && sec != "" {
      line = $0
      sub(/^[[:space:]]+/, "", line)
      if (line !~ /parameter|rosout/) print "  " sec " " line
    }'
done

kill -TERM -- "-$launch_pid" 2>/dev/null || true
wait "$launch_pid" 2>/dev/null || true
sleep 1
