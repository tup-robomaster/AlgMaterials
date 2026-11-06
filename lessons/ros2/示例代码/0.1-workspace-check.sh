#!/usr/bin/env bash
# 0.1 的工作空间检查：打印路径、源码包与已安装包。
set -euo pipefail

ws=${1:-${ROS2_WS:-$HOME/ros2_ws}}
if [[ ! -d "$ws/src" ]]; then
  echo "找不到工作空间：$ws" >&2
  exit 1
fi

echo "workspace: $ws"
echo "src packages:"
(cd "$ws" && colcon list --names-only)
echo "install:"
(cd "$ws" && find install -mindepth 1 -maxdepth 1 -type d -printf '%f\n' | sort)
