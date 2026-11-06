#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes listener >/dev/null 2>&1 &
wait_for node /listener

ros2 run robot_nodes talker 20 >/dev/null 2>&1 &
wait_for node /talker

ros2 node list | grep -E '^/(listener|talker)$' | sort
ros2 topic info /chatter
