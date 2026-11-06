#!/usr/bin/env bash
set -euo pipefail
source "$(dirname "$0")/../lib.sh"

ros2 run robot_nodes lifecycle_talker >/dev/null 2>&1 &
wait_for service /lifecycle_talker/get_state

ros2 lifecycle get /lifecycle_talker
for transition in configure activate deactivate cleanup; do
  ros2 lifecycle set /lifecycle_talker "$transition" >/dev/null
  ros2 lifecycle get /lifecycle_talker
done
