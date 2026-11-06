#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 launch robot_bringup lifecycle_two.launch.xml 2>&1 \
  | grep -E '^\[lifecycle_manager-[0-9]+\]'
