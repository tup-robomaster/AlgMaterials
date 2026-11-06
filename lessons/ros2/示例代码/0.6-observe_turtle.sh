#!/usr/bin/env bash
# 0.6 的观察脚本：turtlesim 已经启动时，列出接口并读一次位姿。
set -euo pipefail

if ! ros2 node list 2>/dev/null | grep -qx '/turtlesim'; then
  echo "先启动 turtlesim：ros2 run turtlesim turtlesim_node" >&2
  exit 1
fi

echo "topics:"
ros2 topic list -t | grep '^/turtle1/' | sort
echo "services:"
ros2 service list -t | grep -E '^/(clear|kill|reset|spawn|turtle1/)' | sort
echo "actions:"
ros2 action list -t | grep '^/turtle1/'
echo "pose:"
ros2 topic echo --once /turtle1/pose
echo "clear:"
ros2 service call /clear std_srvs/srv/Empty
