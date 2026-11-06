#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_answers a1_4 > /tmp/ros2_answer_1_4.out 2>&1 &
listener_pid=$!
sleep 1
ros2 run robot_answers a1_4_words >/dev/null 2>&1
wait "$listener_pid"
cat /tmp/ros2_answer_1_4.out
