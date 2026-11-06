#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes lifecycle_talker >/dev/null 2>&1 &
wait_for service /lifecycle_talker/get_state

ros2 lifecycle get /lifecycle_talker
ros2 lifecycle list /lifecycle_talker
