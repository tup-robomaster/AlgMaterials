#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 launch "$(dirname "$0")/greeter.launch.py" 2>&1 | grep -E '^\[hello_node-[0-9]+\] \[INFO\]'
