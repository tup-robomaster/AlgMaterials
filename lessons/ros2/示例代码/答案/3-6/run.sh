#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes patrol_server >/dev/null 2>&1 &
sleep 1
ros2 run robot_answers a3_6
