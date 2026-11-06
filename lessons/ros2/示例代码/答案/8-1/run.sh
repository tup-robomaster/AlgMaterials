#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes logging_demo --ros-args --log-level logging_demo:=debug
