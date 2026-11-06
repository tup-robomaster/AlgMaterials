#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes lifecycle_talker >/dev/null 2>&1 &
wait_for service /lifecycle_talker/get_state

ros2 lifecycle set /lifecycle_talker configure >/dev/null
ros2 lifecycle set /lifecycle_talker activate >/dev/null
timeout 3 ros2 topic echo --once --field data /chatter 2>/dev/null | head -n1 || true

ros2 lifecycle set /lifecycle_talker deactivate >/dev/null
sleep 0.7
timeout 1.5 ros2 topic echo --once --field data /chatter 2>/dev/null || echo 'no message'
