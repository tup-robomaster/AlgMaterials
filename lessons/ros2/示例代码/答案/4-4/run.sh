#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes executor_probe multi separate 50
