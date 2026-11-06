#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes talker 10 >/dev/null 2>&1 &
sleep 1.5
ros2 topic info --verbose /chatter | grep -E 'Publisher count|Node name: talker|Reliability'
wait
