#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

tmp=$(mktemp -d)
bag="$tmp/two_messages"
trap 'answer_cleanup; rm -rf "$tmp"' EXIT

timeout -s INT 5s ros2 bag record -o "$bag" --include-unpublished-topics /chatter \
  >"$tmp/record.log" 2>&1 &
record_pid=$!
sleep 1

ros2 run robot_nodes talker 2 --ros-args -p wait_for_subscriber:=true
wait "$record_pid" || true

ros2 bag info "$bag" | awk '
  /^Messages:/ {print "Messages: " $2}
  /^Topic information:/ {
    sub(/ \| Serialization Format: cdr/, "")
    print
  }
'
