#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 launch robot_bringup hello.launch.xml 2>&1 | grep -E '^\[hello_node-[0-9]+\] \[INFO\]'
