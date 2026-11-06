#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 pkg xml robot_nodes | grep -E '<depend>std_msgs</depend>'
