#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 topic pub -r 10 /chatter std_msgs/msg/String "{data: probe}" >/dev/null 2>&1 &
pub=$!
wait_for topic /chatter
ros2 topic echo --once --field data /chatter
kill "$pub"
wait "$pub" 2>/dev/null || true
