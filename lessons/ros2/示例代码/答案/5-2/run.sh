#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 launch robot_bringup talker.launch.xml --show-args
