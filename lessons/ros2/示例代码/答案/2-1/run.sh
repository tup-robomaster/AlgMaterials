#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_answers a2_1 >/dev/null 2>&1 &
sleep 1
ros2 service call /multiply example_interfaces/srv/AddTwoInts "{a: 6, b: 7}"
