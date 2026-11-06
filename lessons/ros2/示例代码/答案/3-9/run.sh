#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes patrol_server >/dev/null 2>&1 &
wait_for action /patrol
ros2 action list -t
