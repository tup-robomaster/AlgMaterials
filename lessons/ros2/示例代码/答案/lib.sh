# 答案脚本共用的环境设置。ROS2_WS 指向课程工作空间，
# ANSWER_WS 指向答案节点的工作空间；两者都可以在命令行覆盖。
set -euo pipefail

: "${ROS2_WS:=$HOME/ros2_ws}"
: "${ANSWER_WS:=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/ws}"

export ROS_DOMAIN_ID="${ROS_DOMAIN_ID:-42}"
export RCUTILS_CONSOLE_OUTPUT_FORMAT='[{severity}] [{name}]: {message}'

set +u
# shellcheck disable=SC1091
source /opt/ros/jazzy/setup.bash
# shellcheck disable=SC1091
source "$ROS2_WS/install/setup.bash"
if [[ -f "$ANSWER_WS/install/setup.bash" ]]; then
  # shellcheck disable=SC1091
  source "$ANSWER_WS/install/setup.bash"
fi
set -u

# 先把发现守护进程启动起来。脚本里的第一条 ros2 命令如果赶上守护进程
# 冷启动，会多花近一秒，后面的固定等待时间就不够用了。
ros2 daemon start >/dev/null 2>&1 || true

# 等某个节点、话题、服务、动作或组件容器出现在图里。
# 用法：wait_for node /target_counter
wait_for() {
  local kind=$1 name=$2 i
  for i in $(seq 1 40); do
    if ros2 "$kind" list 2>/dev/null | grep -qx -- "$name"; then
      return 0
    fi
    sleep 0.25
  done
  echo "等待 ${kind} ${name} 超时。" >&2
  return 1
}

answer_cleanup() {
  local jobs job child
  jobs=$(jobs -p || true)
  if [[ -n "$jobs" ]]; then
    # ros2 run 启动的节点是它的子进程。先停子进程，再停 ros2 run；
    # 反过来会让节点被 init 收养，下一次运行 ros2 topic info 会多出一个端点。
    for job in $jobs; do
      for child in $(pgrep -P "$job" || true); do
        pkill -TERM -P "$child" 2>/dev/null || true
        kill "$child" 2>/dev/null || true
      done
      kill "$job" 2>/dev/null || true
    done
    wait 2>/dev/null || true
  fi
}
trap answer_cleanup EXIT
