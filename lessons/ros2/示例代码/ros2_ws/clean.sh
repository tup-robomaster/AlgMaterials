#!/usr/bin/env bash
# 清理课程工作空间的构建产物：build/、install/ 与 log/。
#
# 用法：
#   bash clean.sh              # 清理 $HOME/ros2_ws
#   bash clean.sh 路径         # 清理指定工作空间
#   bash clean.sh 路径 --yes   # 不询问，直接删除
set -euo pipefail

ws=${ROS2_WS:-$HOME/ros2_ws}
assume_yes=0
for arg in "$@"; do
  case "$arg" in
    --yes|-y) assume_yes=1 ;;
    *) ws=$arg ;;
  esac
done

if [[ ! -d "$ws/src" ]]; then
  echo "不是工作空间（缺少 src/）：$ws" >&2
  exit 1
fi

ws=$(cd "$ws" && pwd -P)
targets=()
for name in build install log; do
  if [[ -e "$ws/$name" ]]; then
    targets+=("$ws/$name")
  fi
done

if [[ ${#targets[@]} -eq 0 ]]; then
  echo "没有需要清理的目录：$ws"
  exit 0
fi

echo "工作空间：$ws"
du -sh "${targets[@]}" 2>/dev/null || true
if [[ $assume_yes -eq 0 ]]; then
  read -r -p "删除以上目录？[y/N] " answer
  case "$answer" in
    y|Y) ;;
    *) echo "已取消。"; exit 0 ;;
  esac
fi

rm -rf -- "${targets[@]}"
echo "已清理：${targets[*]}"
