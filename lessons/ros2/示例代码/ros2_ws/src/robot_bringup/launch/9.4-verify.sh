#!/usr/bin/env bash
set -euo pipefail

steps=${1:-2}
max_distance=${2:-0.0}
log=$(mktemp)
trap 'rm -f "$log"' EXIT

ros2 launch robot_bringup 9.4-patrol.launch.py \
  steps_per_target:="$steps" max_distance:="$max_distance" >"$log" 2>&1

grep -E 'publish:|skip:|duplicate ignored:|goal accepted:|controller done:' "$log" |
  sed -E 's/^\[([a-z_]+)-[0-9]+\] \[INFO\] (\[[0-9.]+\] )?\[[a-z_]+\]: /[\1]: /'
