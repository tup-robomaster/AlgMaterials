#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

prefix=$(ros2 pkg prefix robot_bringup)
find "$prefix/share/robot_bringup/launch" -maxdepth 1 \( -type f -o -type l \) | sort |
  sed "s|^$prefix|\$HOME/ros2_ws/install/robot_bringup|"
