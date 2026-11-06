#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run turtlesim turtlesim_node >/dev/null 2>&1 &
sleep 2
ros2 topic list | grep '^/turtle1/' | sort
