#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run turtlesim turtlesim_node >/dev/null 2>&1 &
sleep 2
ros2 service call /spawn turtlesim/srv/Spawn \
  "{x: 2.0, y: 2.0, theta: 0.0, name: 'turtle2'}" >/dev/null
sleep 0.5
ros2 topic list | grep '^/turtle2/' | sort
