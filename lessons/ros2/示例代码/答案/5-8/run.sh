#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 launch robot_bringup conditional.launch.xml with_hello:=false 2>&1 | grep -E '^\[talker-[0-9]+\] \[INFO\]'
