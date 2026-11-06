#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes add_server >/dev/null 2>&1 &
sleep 1
ros2 run robot_answers a2_3
