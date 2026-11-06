#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 pkg list | grep -E '^robot_(bringup|interfaces|nodes)$' | sort
ros2 pkg executables robot_nodes | grep ' hello_node$'
