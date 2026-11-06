#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

g++ -std=c++17 \
  -I "$ROS2_WS/src/robot_nodes/include" \
  "$(dirname "$0")/test_negative.cpp" \
  "$ROS2_WS/src/robot_nodes/src/area.cpp" \
  -lgtest -lgtest_main -lpthread \
  -o "$tmp/test_negative"

"$tmp/test_negative"
