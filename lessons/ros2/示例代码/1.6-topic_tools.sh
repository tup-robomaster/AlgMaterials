#!/usr/bin/env bash
# 1.6 的话题诊断：启动一个 10 Hz 的 CLI 发布者，依次观察列表、类型、端点、
# 一条消息与频率。频率数值随运行环境变化，页面标为真实输出。
set -euo pipefail

pub=
cleanup() {
  if [[ -n "$pub" ]]; then
    kill "$pub" 2>/dev/null || true
    wait "$pub" 2>/dev/null || true
  fi
}
trap cleanup EXIT

ros2 topic pub -r 10 /diag_chatter std_msgs/msg/String "{data: diag}" \
  >/dev/null 2>&1 &
pub=$!
sleep 2

echo "list:"
ros2 topic list -t | grep '^/diag_chatter'
echo "info:"
ros2 topic info /diag_chatter
echo "echo:"
ros2 topic echo --once /diag_chatter
sleep 2
echo "hz:"
timeout --signal=INT 6 ros2 topic hz --window 10 /diag_chatter 2>/dev/null \
  | grep -m1 -A1 '^average rate' || true
