#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes hello_node --ros-args -r __node:=worker -r __ns:=/demo >/dev/null 2>&1 &
sleep 0.5
ros2 node list | grep '^/demo/worker$'
wait
