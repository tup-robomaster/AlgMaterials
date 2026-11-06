#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes lifecycle_talker >/dev/null 2>&1 &
ros2 run robot_nodes lifecycle_listener >/dev/null 2>&1 &
wait_for service /lifecycle_talker/change_state
wait_for service /lifecycle_listener/change_state

ros2 lifecycle set /lifecycle_talker configure >/dev/null
ros2 lifecycle set /lifecycle_listener configure >/dev/null
sleep 0.3
ros2 topic info /chatter | grep -E '^(Publisher|Subscription) count'
